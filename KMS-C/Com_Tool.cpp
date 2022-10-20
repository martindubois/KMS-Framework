
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Com_Port.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Console/Color.h>
#include <KMS/Installer.h>

#include <KMS/Com/Tool.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-ComTool.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_DATA_FILE("DataFile = false|true");

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CreateTimestamp(char* aOut, unsigned int aOutSize_byte);

static void Dump(FILE* aOut, const void* aIn, unsigned int aInSize_byte);

static unsigned int ToData(const char* aIn, void* aOut, unsigned int aOutSize_byte);

static unsigned int ToFlags(const char* aIn);

static unsigned int ToFrameT(const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);

namespace KMS
{
    namespace Com
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int Tool::FLAG_DISPLAY       = 0x00000001;
        const unsigned int Tool::FLAG_DUMP          = 0x00000002;
        const unsigned int Tool::FLAG_EXIT_ON_ERROR = 0x00000004;
        const unsigned int Tool::FLAG_FRAME_T       = 0x00000008;
        const unsigned int Tool::FLAG_TIMESTAMP     = 0x00000010;
        const unsigned int Tool::FLAG_WRITE         = 0x00000020;

        int Tool::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);

            int lResult = __LINE__;

            try
            {
                KMS::Cfg::Configurator  lC;
                KMS::Com::Tool          lT;
                KMS::Installer          lInstaller;

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lT);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                Dbg::gLog.CloseLogFiles();

                lInstaller.Run();

                lResult = lT.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Tool::Tool() : mDataFile(NULL, "")
        {
            mDataFile.SetMode("wb");

            AddEntry("DataFile", &mDataFile, false, &MD_DATA_FILE);

            AddEntry("Port", &mPort, false);
        }

        void Tool::Receive(unsigned int aSize_byte, unsigned int aFlags)
        {
            assert(LINE_LENGTH > aSize_byte);

            char lData[LINE_LENGTH];

            unsigned int lSize_byte;

            if (0 == aSize_byte)
            {
                lSize_byte = mPort.Read(lData, sizeof(lData), 0);
            }
            else
            {
                lSize_byte = mPort.Read(lData, aSize_byte, Com::Port::FLAG_READ_ALL);
            }

            lData[lSize_byte] = '\0';

            if (0 < lSize_byte)
            {
                std::cout << KMS::Console::Color::GREEN;

                DisplayDumpWrite(lData, lSize_byte, aFlags, "Receive");

                std::cout << KMS::Console::Color::WHITE << std::flush;
            }
        }

        void Tool::ReceiveAndVerify(const void* aIn, unsigned int aInSize_byte, unsigned int aFlags)
        {
            assert(0 < aInSize_byte);
            assert(LINE_LENGTH > aInSize_byte);

            char lIn[LINE_LENGTH];
            unsigned int lInSize_byte;

            if (0 != (aFlags & FLAG_FRAME_T))
            {
                lInSize_byte = ToFrameT(aIn, aInSize_byte, lIn, sizeof(lIn));
            }
            else
            {
                lInSize_byte = aInSize_byte;
                memcpy(lIn, aIn, lInSize_byte);
            }

            char lData[LINE_LENGTH];

            unsigned int lSize_byte = mPort.Read(lData, lInSize_byte, Com::Port::FLAG_READ_ALL);

            lData[lSize_byte] = '\0';

            bool lMatch = (lInSize_byte == lSize_byte);
            if (lMatch)
            {
                lMatch = 0 == memcmp(lIn, lData, lSize_byte);
            }

            if (lMatch)
            {
                std::cout << KMS::Console::Color::GREEN;

                DisplayDumpWrite(lData, lSize_byte, aFlags, "Receive and verify PASSED");

                std::cout << KMS::Console::Color::WHITE << std::flush;
            }
            else
            {
                std::cout << KMS::Console::Color::RED;

                DisplayDumpWrite(lData, lSize_byte, aFlags | FLAG_DUMP | FLAG_TIMESTAMP, "Receive and verify FAILED");

                std::cout << KMS::Console::Color::WHITE << std::flush;
            }
        }

        void Tool::Send(const void* aIn, unsigned int aInSize_byte, unsigned int aFlags)
        {
            assert(0 < aInSize_byte);
            assert(LINE_LENGTH > aInSize_byte);

            char lIn[LINE_LENGTH];
            unsigned int lInSize_byte;

            if (0 != (aFlags & FLAG_FRAME_T))
            {
                lInSize_byte = ToFrameT(aIn, aInSize_byte, lIn, sizeof(lIn));
            }
            else
            {
                lInSize_byte = aInSize_byte;
                memcpy(lIn, aIn, lInSize_byte);
            }

            lIn[lInSize_byte] = '\0';

            mPort.Write(lIn, lInSize_byte);

            std::cout << KMS::Console::Color::BLUE;

            DisplayDumpWrite(lIn, lInSize_byte, aFlags, "Send");

            std::cout << KMS::Console::Color::WHITE << std::flush;
        }

        // ===== CLI::Tool ==================================================

        void Tool::DisplayDumpWrite(const void* aIn, unsigned int aInSize_byte, unsigned int aFlags, const char* aOp)
        {
            assert(NULL != aIn);
            assert(NULL != aOp);

            char lNow[NAME_LENGTH];

            CreateTimestamp(lNow, sizeof(lNow));

            if (0 != (aFlags & FLAG_DISPLAY))
            {
                if (0 != (aFlags & FLAG_TIMESTAMP)) { std::cout << aOp << " " << lNow << "\n"; }

                std::cout << reinterpret_cast<const char*>(aIn) << "\n";
            }

            if (0 != (aFlags & FLAG_DUMP))
            {
                if ((0 != (aFlags & FLAG_TIMESTAMP)) && (0 == (aFlags & FLAG_DISPLAY))) { std::cout << aOp << " " << lNow << "\n"; }

                Dump(stdout, aIn, aInSize_byte);
            }

            if ((0 != (aFlags & FLAG_WRITE)) && (NULL != mDataFile.Get()))
            {
                fprintf(mDataFile, "%s %s\n", aOp, lNow);

                Dump(mDataFile, aIn, aInSize_byte);
            }
        }

        void Tool::DisplayHelp(FILE* aFile) const
        {
            assert(NULL != aFile);

            fprintf(aFile,
                "ClearDTR\n"
                "ClearRTS\n"
                "Receive [Flags] [Size_byte]\n"
                "ReceiveAndVerify {Flags} {Expected}\n"
                "ReceiveAndVerify_Hex {Flags} {Expected}\n"
                "ResetDataFile\n"
                "Send {Flags} {Data}\n"
                "Send_Hex {Flags} {Data}\n"
                "SetDTR\n"
                "SetRTS\n"
                "StartReceive [Flags]\n");

            CLI::Tool::DisplayHelp(aFile);
        }

        void Tool::ExecuteCommand(const char* aC)
        {
            char lData [LINE_LENGTH];
            char lFlags[LINE_LENGTH];
            unsigned int lSize_byte = 0;

            if      (0 == strcmp(aC, "ClearDTR")) { mPort.SetDTR(false); }
            else if (0 == strcmp(aC, "ClearRTS")) { mPort.SetRTS(false); }
            else if (0 == strcmp(aC, "Receive" )) { Receive(0, 0); }
            else if (0 == strcmp(aC, "SetDTR"  )) { mPort.SetDTR(true); }
            else if (0 == strcmp(aC, "SetRTS"  )) { mPort.SetRTS(true); }
            else if (2 == sscanf_s(aC, "ReceiveAndVerify_Hex %[0A-Z_|] %[^\n\r\t]", lFlags SizeInfo(lFlags), &lData SizeInfo(lData)))
            {
                ReceiveAndVerify_Hex(lData, ToFlags(lFlags));
            }
            else if (2 == sscanf_s(aC, "ReceiveAndVerify %[0A-Z_|] %[^\n\r\t]"    , lFlags SizeInfo(lFlags), &lData SizeInfo(lData)))
            {
                ReceiveAndVerify(lData, static_cast<unsigned int>(strlen(lData)), ToFlags(lFlags));
            }
            else if (1 <= sscanf_s(aC, "Receive %[0A-Z_|] %u"                     , lFlags SizeInfo(lFlags), &lSize_byte))
            {
                Receive(lSize_byte, ToFlags(lFlags));
            }
            else if (2 == sscanf_s(aC, "Send_Hex %[0A-Z_|] %[^\n\r\t]"            , lFlags SizeInfo(lFlags), &lData SizeInfo(lData)))
            {
                Send_Hex(lData, ToFlags(lFlags));
            }
            else if (2 == sscanf_s(aC, "Send %[0A-Z_|] %[^\n\r\t]"                , lFlags SizeInfo(lFlags), &lData SizeInfo(lData)))
            {
                Send(lData, static_cast<unsigned int>(strlen(lData)), ToFlags(lFlags));
            }
            else
            {
                CLI::Tool::ExecuteCommand(aC);
            }
        }

        int Tool::Run()
        {
            mPort.Connect(KMS::Dev::Device::FLAG_READ_ACCESS | KMS::Dev::Device::FLAG_WRITE_ACCESS);

            return CLI::Tool::Run();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Tool::ReceiveAndVerify_Hex(const char* aIn, unsigned int aFlags)
        {
            uint8_t lData[LINE_LENGTH];

            unsigned int lSize_byte = ToData(aIn, lData, sizeof(lData));

            ReceiveAndVerify(lData, lSize_byte, aFlags);
        }

        void Tool::Send_Hex(const char* aIn, unsigned int aFlags)
        {
            uint8_t lData[LINE_LENGTH];

            unsigned int lSize_byte = ToData(aIn, lData, sizeof(lData));

            Send(lData, lSize_byte, aFlags);
        }

    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void CreateTimestamp(char* aOut, unsigned int aOutSize_byte)
{
    assert(NULL != aOut);

    SYSTEMTIME lTime;

    GetLocalTime(&lTime);

    sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u-%u-%u %u:%u:%u.%03u",
        lTime.wYear, lTime.wMonth, lTime.wDay, lTime.wHour, lTime.wMinute, lTime.wSecond, lTime.wMilliseconds);
}

void Dump(FILE* aOut, const void* aIn, unsigned int aInSize_byte)
{
    assert(NULL != aOut);
    assert(NULL != aIn);

    const uint8_t* lIn = static_cast<const uint8_t*>(aIn);

    for (unsigned int i = 0; i < aInSize_byte; i++)
    {
        fprintf(aOut, " %02x", lIn[i]);
    }

    fprintf(aOut, "\n");
}

unsigned int ToData(const char* aIn, void* aOut, unsigned int aOutSize_byte)
{
    unsigned int lResult_byte = 0;

    const char* lIn  = aIn;
    uint8_t   * lOut = reinterpret_cast<uint8_t*>(aOut);

    uint8_t lByte = 0;

    for (;;)
    {
        switch (*lIn)
        {
        case '\0': lOut[lResult_byte] = lByte; lResult_byte++; return lResult_byte;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F': lByte <<= 4; lByte |= *lIn - 'A' + 10; break;

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f': lByte <<= 4; lByte |= *lIn - 'a' + 10; break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': lByte <<= 4; lByte |= *lIn - '0'; break;

        case ' ': lOut[lResult_byte] = lByte; lByte = 0; lResult_byte++; break;

        default: KMS_EXCEPTION(COM_DATA_INVALID, "Invalid data format", aIn);
        }

        lIn++;
    }
}

unsigned int ToFrameT(const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
{
    assert(NULL != aIn);
    assert(NULL != aOut);

    KMS_EXCEPTION_ASSERT(aInSize_byte + 4 <= aOutSize_byte, COM_OUTPUT_TOO_SHORT, "The output buffer is too short", aInSize_byte);

    uint8_t* lOut = reinterpret_cast<uint8_t*>(aOut);
    unsigned int lResult_byte = 0;

    lOut[lResult_byte] = 0x7e; lResult_byte++;
    memcpy(lOut + lResult_byte, aIn, aInSize_byte); lResult_byte += aInSize_byte;

    uint16_t lCRC = 0xffff;

    for (unsigned int i = 0; i < lResult_byte; i++)
    {
        uint8_t lByte = lOut[i];

        lByte ^= lCRC;
        lByte ^= lByte << 4;

        lCRC = ((lByte << 8) | (0xff & (lCRC >> 8))) ^ (lByte >> 4) ^ (lByte << 3);
    }

    memcpy(lOut + lResult_byte, &lCRC, sizeof(lCRC)); lResult_byte += sizeof(lCRC);
    lOut[lResult_byte] = 0x7f; lResult_byte++;

    return lResult_byte;
}

unsigned int ToFlags(const char* aIn)
{
    unsigned int lResult = 0;

    if (NULL != strstr(aIn, "DISPLAY"      )) { lResult |= KMS::Com::Tool::FLAG_DISPLAY; }
    if (NULL != strstr(aIn, "DUMP"         )) { lResult |= KMS::Com::Tool::FLAG_DUMP; }
    if (NULL != strstr(aIn, "EXIT_ON_ERROR")) { lResult |= KMS::Com::Tool::FLAG_EXIT_ON_ERROR; }
    if (NULL != strstr(aIn, "FRAME_T"      )) { lResult |= KMS::Com::Tool::FLAG_FRAME_T; }
    if (NULL != strstr(aIn, "TIMESTAMP"    )) { lResult |= KMS::Com::Tool::FLAG_TIMESTAMP; }
    if (NULL != strstr(aIn, "WRITE"        )) { lResult |= KMS::Com::Tool::FLAG_WRITE; }

    return lResult;
}
