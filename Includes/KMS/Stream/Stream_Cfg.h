
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Stream/Stream_Cfg.h
// Library   KMS-C

#pragma once

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/CLI/CommandLine.h>
#include <KMS/Com/Port.h>
#include <KMS/Net/Socket_Client.h>
#include <KMS/Stream/IStream.h>

namespace KMS
{
    namespace Stream
    {

        enum class StreamType
        {
            COM = 0,
            TCP,

            QTY
        };

        class Stream_Cfg
        {

        public:

            Stream_Cfg(Cfg::Configurator* aConfigurator, StreamType aDefault);

            ~Stream_Cfg();

            IStream* GetStream();

            StreamType GetStreamType() const;

            void ParseArguments(CLI::CommandLine* aCmdLine);

        protected:

            Cfg::Configurator* GetConfigurator();

            Com::Port          mPort;
            Net::Socket_Client mSocket;

        private:

            Cfg::Configurator* mConfigurator;
            IStream          * mStream;
            StreamType         mType;

        };

    }
}