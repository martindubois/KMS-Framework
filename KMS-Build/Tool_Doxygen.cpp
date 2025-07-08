
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Doxygen.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Phase.h"
#include "Tool_Executable.h"
#include "Tool_Doxygen.h"

using namespace KMS;

#define FILE_EXT_TXT ".txt"

#ifdef _KMS_WINDOWS_
    #define FOLDER "doxygen\\bin"
#endif

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define ALLOWED_TIME_ms (1000 * 60 * 5) // 5 minutes

#define EXECUTABLE ("doxygen")

static const char* LANGUAGES[] = { "en", "fr" };

#define LANGUAGE_QTY (sizeof(LANGUAGES) / sizeof(LANGUAGES[0]))

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CreateTool(ToolList* aTools, const Config& aCfg, const char* aLanguage);

namespace Tool_Doxygen
{

    // Functions
    // //////////////////////////////////////////////////////////////////////////

    void CreateTools(ToolList* aTools, const Config& aCfg)
    {
        for (unsigned int i = 0; i < LANGUAGE_QTY; i++)
        {
            CreateTool(aTools, aCfg, LANGUAGES[i]);
        }
    }

}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void CreateTool(ToolList* aTools, const Config& aCfg, const char* aLanguage)
{
    assert(nullptr != aLanguage);

    char lFileName[PATH_LENGTH];

    sprintf_s(lFileName, "DoxyFile_%s" FILE_EXT_TXT, aLanguage);

    if (File::Folder::CURRENT.DoesFileExist(lFileName))
    {
        #ifdef _KMS_LINUX_
            auto lFolder = new File::Folder(File::Folder::NONE);
        #endif

        #ifdef _KMS_WINDWS_
            auto lFolder = new File::Folder(File::Folder::PROGRAM_FILES, FOLDER);
        #endif

        auto lTool = Tool_Executable::CreateTool(aTools, aCfg, lFolder, EXECUTABLE, ALLOWED_TIME_ms, Phase::VERIFY, Phase::GENERATE_DOC);

        lTool->AddArgument(lFileName);
    }
}
