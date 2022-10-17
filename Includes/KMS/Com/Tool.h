
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Com/Port.h
// Library   KMS-C

#pragma once

// ===== Includes ===========================================================
#include <KMS/Com/Port.h>
#include <KMS/CLI/Tool.h>
#include <KMS/DI/File.h>

namespace KMS
{
    namespace Com
    {

        class Tool : public CLI::Tool
        {

        public:

            static const unsigned int FLAG_DISPLAY;
            static const unsigned int FLAG_DUMP;
            static const unsigned int FLAG_EXIT_ON_ERROR;
            static const unsigned int FLAG_FRAME_T;
            static const unsigned int FLAG_TIMESTAMP;
            static const unsigned int FLAG_WRITE;

            static int Main(int aCount, const char** aVector);

            Tool();

            void Receive(unsigned int aSize_byte = 0, unsigned int aFlags = 0);

            void ReceiveAndVerify(const void* aIn, unsigned int aInSize_byte, unsigned int aFlags = 0);

            void ResetDataFile();

            void Send(const void* aIn, unsigned int aInSize_byte, unsigned int aFlags = 0);

            // ===== CLI::Tool ==============================================
            virtual void DisplayHelp(FILE* aOut) const;
            virtual void ExecuteCommand(const char* aC);
            virtual int  Run();

        private:

            Tool(const Tool&);

            const Tool operator = (const Tool&);

            Port mPort;

            void DisplayDumpWrite(const void* aIn, unsigned int aInSize_byte, unsigned int aFlags, const char* aOp);

            void ReceiveAndVerify_Hex(const char* aIn, unsigned int aFlags);

            void Send_Hex(const char* aIn, unsigned int aFlags);

            // ===== Configurable attributes ================================
            DI::File mDataFile;

        };

    }
}
