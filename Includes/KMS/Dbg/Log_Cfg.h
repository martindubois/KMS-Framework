
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Log_Cfg.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Enum.h>
#include <KMS/DI/Folder.h>

namespace KMS
{
    namespace Dbg
    {

        class Log_Cfg : public DI::Dictionary
        {

        public:

            DI::Enum_Ptr<LogFile::Level  , LogFile::LEVEL_NAMES   > mConsoleLevel;
            DI::Enum_Ptr<Log::ConsoleMode, Log::CONSOLE_MODE_NAMES> mConsoleMode;
            DI::Enum_Ptr<LogFile::Level  , LogFile::LEVEL_NAMES   > mFileLevel;
            DI::Folder_Ptr                                          mFolder;

            Log_Cfg(Log* aLog);

        private:

            NO_COPY(Log_Cfg);

            // ===== Callbacks ==============================================
            const Callback<Log_Cfg> ON_FOLDER_CHANGED;
            unsigned int OnFolderChanged(void* aSender, void* aData);

            Log* mLog;

        };

    }
}
