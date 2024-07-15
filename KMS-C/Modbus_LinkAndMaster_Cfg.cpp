
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Modbus_LinkAndMaster_Cfg.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <winsock2.h>

// ===== Includes ===========================================================
#include <KMS/Enum.h>

#include <KMS/Modbus/LinkAndMaster_Cfg.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const char* LINK_NAMES[] = { "COM", "TCP" };

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        LinkAndMaster_Cfg::LinkAndMaster_Cfg(Cfg::Configurator* aConfigurator, Stream::StreamType aDefault)
            : Stream_Cfg(aConfigurator, aDefault)
            , mMaster(nullptr)
            , mM_Cfg(nullptr)
        {
            mPort.SetSpeed(38400);
        }

        LinkAndMaster_Cfg::~LinkAndMaster_Cfg()
        {
            if (nullptr != mMaster)
            {
                if (nullptr != mM_Cfg)
                {
                    delete mM_Cfg;
                }

                delete mMaster;
            }
        }

        Master* LinkAndMaster_Cfg::GetMaster() { assert(nullptr != mMaster); return mMaster; }

        unsigned int LinkAndMaster_Cfg::ParseArguments(unsigned int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(nullptr != aVector);

            auto lResult = Stream_Cfg::ParseArguments(aCount, aVector);

            assert(nullptr == mMaster);
            assert(nullptr == mM_Cfg);


            switch (GetStreamType())
            {
            case Stream::StreamType::COM: mMaster = new Master_IDevice(&mPort  ); break;
            case Stream::StreamType::TCP: mMaster = new Master_TCP    (&mSocket); break;

            default: assert(false);
            }

            mM_Cfg = new Master_Cfg(mMaster);

            GetConfigurator()->AddConfigurable(mM_Cfg);

            return lResult;
        }

    }
}
