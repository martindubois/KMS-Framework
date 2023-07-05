
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Log_Cfg.cpp

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

#define ON_FOLDER_CHANGED (1)

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Log_Cfg::CONSOLE_MODE_NAMES[] = { "DEBUG", "USER" };

        Log_Cfg::Log_Cfg(Dbg::Log* aLog)
            : mLog(aLog)
            , mConsoleLevel(&aLog->mConsoleLevel)
            , mConsoleMode (&aLog->mConsoleMode )
            , mFileLevel   (&aLog->mFileLevel   )
            , mFolder      (&aLog->mFolder      )
        {
            mFolder.mOnChanged.Set(this, ON_FOLDER_CHANGED);

            AddEntry("Log_ConsoleLevel", &mConsoleLevel, false, &MD_LOG_CONSOLE_LEVEL);
            AddEntry("Log_ConsoleMode" , &mConsoleMode , false, &MD_LOG_CONSOLE_MODE);
            AddEntry("Log_FileLevel"   , &mFileLevel   , false, &MD_LOG_FILE_LEVEL);
            AddEntry("Log_Folder"      , &mFolder      , false, &MD_LOG_FOLDER);
        }

        // ===== Msg::IReceiver =============================================

        unsigned int Log_Cfg::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            assert(NULL != mLog);

            auto lResult = Msg::IReceiver::MSG_IGNORED;

            switch (aCode)
            {
            case ON_FOLDER_CHANGED: mLog->CloseLogFiles(); lResult = 0; break;

            default: assert(false);
            }

            return lResult;
        }

    }
}
