
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Editor.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/String.h>
#include <KMS/Text/File_ASCII.h>

#include <KMS/Build/Editor.h>

// Data types
// //////////////////////////////////////////////////////////////////////////

class Operation
{

public:

    bool IsValid(char* aMsg, unsigned int aMsgSize_byte) const;

    void Execute(unsigned int aIndex, const KMS::Version& aVersion);

    bool Parse(const char* aIn);

private:

    char mFile   [FILE_LENGTH];
    char mRegEx  [LINE_LENGTH];
    char mReplace[LINE_LENGTH];

};

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static bool IsOperationValid(const char* aIn, char* aMsg, unsigned int aMsgSize_byte);

static KMS::String_ASCII ProcessReplaceLine(const char* aIn, const KMS::Version& aVersion);

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool Editor_Config::IsValid(char* aMsg, unsigned int aMsgSize_byte) const
        {
            bool lResult = true;

            for (auto lStr : mOperations)
            {
                if (!IsOperationValid(lStr.c_str(), aMsg, aMsgSize_byte))
                {
                    return false;
                }
            }

            auto lVersionFile = 0 < strlen(mVersionFile);

            if (mVersion.IsZero())
            {
                if (!lVersionFile)
                {
                    if ((nullptr != aMsg) && (0 < aMsgSize_byte))
                    {
                        strcpy_s(aMsg SizeInfoV(aMsgSize_byte), "No version and no version file name");
                    }
                    return false;
                }

                if (!File::Folder::CURRENT.DoesFileExist(mVersionFile))
                {
                    if ((nullptr != aMsg) && (0 < aMsgSize_byte))
                    {
                        sprintf_s(aMsg SizeInfoV(aMsgSize_byte), "The file %s does not exist", mVersionFile);
                    }
                    return false;
                }
            }
            else
            {
                if (lVersionFile)
                {
                    if ((nullptr != aMsg) && (0 < aMsgSize_byte))
                    {
                        strcpy_s(aMsg SizeInfoV(aMsgSize_byte), "Version and version file");
                    }
                    return false;
                }
            }

            return true;
        }

        void Editor_Config::SetDefault()
        {
            memset(&mVersionFile, 0, sizeof(mVersionFile));
        }

        void Editor_Config::Validate() const
        {
            char lMsg[LINE_LENGTH];

            auto lRet = IsValid(lMsg, sizeof(lMsg));

            KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_CONFIG, "Invalid configuration", lMsg);
        }

        void Editor::Execute()
        {
            Version lVersion;

            if (0 < strlen(mConfig.mVersionFile))
            {
                lVersion = Version(File::Folder::CURRENT, mConfig.mVersionFile);
            }
            else
            {
                lVersion = mConfig.mVersion;
            }

            unsigned int lIndex = 0;

            for (auto lStr : mConfig.mOperations)
            {
                Operation lOp;

                lOp.Parse(lStr.c_str());

                lIndex++;

                lOp.Execute(lIndex, lVersion);
            }
        }
    }
}

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

bool Operation::IsValid(char* aMsg, unsigned int aMsgSize_byte) const
{
    if (!File::Folder::CURRENT.DoesFileExist(mFile))
    {
        if ((nullptr != aMsg) && (0 < aMsgSize_byte))
        {
            sprintf_s(aMsg SizeInfoV(aMsgSize_byte), "The file %s does not exist", mFile);
        }
        return false;
    }

    return true;
}

void Operation::Execute(unsigned int aIndex, const Version& aVersion)
{
    auto lReplaceStr = ProcessReplaceLine(mReplace, aVersion);

    Text::File_ASCII lText;

    lText.Read(KMS::File::Folder::CURRENT, mFile);

    auto lCount = lText.ReplaceLines(mRegEx, lReplaceStr.c_str());
    if (0 == lCount)
    {
        std::cout << Console::Color::RED;
        {
            std::cout << "Edit operation " << aIndex << " - 0 line replaced";
        }
        std::cout << Console::Color::WHITE;
    }
    else
    {
        std::cout << "Edit operation " << aIndex << " - " << lCount << " line replaced";

        File::Folder::CURRENT.Backup(mFile);

        lText.Write(KMS::File::Folder::CURRENT, mFile);
    }

    std::cout << std::endl;

}

bool Operation::Parse(const char* aIn)
{
    auto lRet = sscanf_s(aIn, "%[^;];%[^;];%[^\n\r]", mFile SizeInfo(mFile), mRegEx SizeInfo(mRegEx), mReplace SizeInfo(mReplace));

    return 3 == lRet;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

bool IsOperationValid(const char* aIn, char* aMsg, unsigned int aMsgSize_byte)
{
    Operation lOp;

    if (!lOp.Parse(aIn))
    {
        if ((nullptr != aMsg) && (0 < aMsgSize_byte))
        {
            strcpy_s(aMsg SizeInfoV(aMsgSize_byte), "Invalid operation format");
        }
        return false;
    }

    return lOp.IsValid(aMsg, aMsgSize_byte);
}

String_ASCII ProcessReplaceLine(const char* aIn, const Version& aVersion)
{
    // --> INIT <--+
    //      |      |
    //      +--> PROCESS
    static const unsigned int STATE_INIT    = 0;
    static const unsigned int STATE_PROCESS = 1;

    assert(nullptr != aIn);

    auto         lIn = aIn;
    String_ASCII lResult;
    unsigned int lState = STATE_INIT;
    auto         lType = aVersion.GetType();

    if (0 >= strlen(lType))
    {
        lType = nullptr;
    }

    for (;;)
    {
        switch (lState)
        {
        case STATE_INIT:
            switch (*lIn)
            {
            case '\0': return lResult;
            case '{': lState = STATE_PROCESS; break;
            default: lResult += *lIn;
            }
            break;

        case STATE_PROCESS:
            switch (*lIn)
            {
            case '\0': KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid edition operation", aIn);
            case '}': lState = STATE_INIT; break;
            case 'M': lResult += aVersion.GetMajor(); break;
            case 'm': lResult += aVersion.GetMinor(); break;
            case 'B': lResult += aVersion.GetBuild(); break;
            case 'C': lResult += aVersion.GetCompat(); break;

            case 'T':
                if (nullptr != lType)
                {
                    lResult += "-";
                    lResult += aVersion.GetType();
                }
                break;

            default: lResult += *lIn;
            }
            break;

        default: assert(false);
        }

        lIn++;
    }
}
