
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log_Cfg.cpp

// TEST COVERAGE 2023-08-06 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Dbg/Log_Cfg.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_LOG_CONSOLE_LEVEL("Log_ConsoleLevel = NOISE | INFO | WARNING | ERROR | NONE");
static const KMS::Cfg::MetaData MD_LOG_CONSOLE_MODE ("Log_ConsoleMode = DEBUG | USER");
static const KMS::Cfg::MetaData MD_LOG_FILE_LEVEL   ("Log_FileLevel = NOISE | INFO | WARNING | ERROR | NONE");
static const KMS::Cfg::MetaData MD_LOG_FOLDER       ("Log_Folder = {Path}");

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Log_Cfg::Log_Cfg(Dbg::Log* aLog)
            : mLog(aLog)
            , mConsoleLevel(&aLog->mConsoleLevel)
            , mConsoleMode (&aLog->mConsoleMode )
            , mFileLevel   (&aLog->mFileLevel   )
            , mFolder      (&aLog->mFolder      )
            // ===== Callbacks ==============================================
            , ON_FOLDER_CHANGED(this, &Log_Cfg::OnFolderChanged)
        {
            mFolder.mOnChanged = &ON_FOLDER_CHANGED;

            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mConsoleLevel, false); AddEntry("Log_ConsoleLevel", lEntry, &MD_LOG_CONSOLE_LEVEL);
            lEntry.Set(&mConsoleMode , false); AddEntry("Log_ConsoleMode" , lEntry, &MD_LOG_CONSOLE_MODE);
            lEntry.Set(&mFileLevel   , false); AddEntry("Log_FileLevel"   , lEntry, &MD_LOG_FILE_LEVEL);
            lEntry.Set(&mFolder      , false); AddEntry("Log_Folder"      , lEntry, &MD_LOG_FOLDER);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Callbacks ==================================================

        // NOT TESTED
        unsigned int Log_Cfg::OnFolderChanged(void*, void*)
        {
            mLog->CloseLogFiles();

            return 0;
        }

    }
}
