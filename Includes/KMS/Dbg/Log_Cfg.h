
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Log_Cfg.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Enum.h>
#include <KMS/DI/Folder.h>

namespace KMS
{
    namespace Dbg
    {

        class Log_Cfg : public DI::Dictionary, public Msg::IReceiver
        {

        public:

            static const char* CONSOLE_MODE_NAMES[];

            Log_Cfg(Log* aLog);

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

            // ===== Configurable attributes ================================
            DI::Enum_Ptr<LogFile::Level  , LogFile::LEVEL_NAMES> mConsoleLevel;
            DI::Enum_Ptr<Log::ConsoleMode, CONSOLE_MODE_NAMES  > mConsoleMode;
            DI::Enum_Ptr<LogFile::Level  , LogFile::LEVEL_NAMES> mFileLevel;
            DI::Folder_Ptr                                       mFolder;

        private:

            NO_COPY(Log_Cfg);

            Log* mLog;

        };

    }
}
