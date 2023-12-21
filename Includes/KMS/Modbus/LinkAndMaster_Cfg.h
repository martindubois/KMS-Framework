
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/LinkAndMaster_Cfg.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Com/Port.h>
#include <KMS/Modbus/Master_CFG.h>
#include <KMS/Modbus/Master_IDevice.h>
#include <KMS/Modbus/Master_TCP.h>

namespace KMS
{
    namespace Modbus
    {

        class LinkAndMaster_Cfg
        {

        public:

            enum class Link
            {
                COM = 0,
                TCP,

                QTY
            };

            LinkAndMaster_Cfg(Cfg::Configurator* aConfigurator, Link aDefault);

            ~LinkAndMaster_Cfg();

            Master* GetMaster();

            unsigned int ParseArguments(unsigned int aCount, const char** aVector);

        private:

            Cfg::Configurator* mConfigurator;
            Link               mLink;
            Master           * mMaster;
            Master_Cfg       * mM_Cfg;
            Master_IDevice     mM_Com;
            Master_TCP         mM_TCP;
            Com::Port          mPort;

        };

    }
}
