
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

        LinkAndMaster_Cfg::LinkAndMaster_Cfg(Cfg::Configurator* aConfigurator, Link aDefault)
            : mConfigurator(aConfigurator)
            , mLink(aDefault)
            , mMaster(nullptr)
            , mM_Cfg(nullptr)
            , mM_Com(&mPort)
        {
            assert(nullptr != aConfigurator);
            assert(Link::QTY > aDefault);

            mPort.SetSpeed(38400);

            auto lSocket = mM_TCP.GetSocket();
            assert(nullptr != lSocket);

            lSocket->mLocalAddress.Set("0.0.0.0");
        }

        LinkAndMaster_Cfg::~LinkAndMaster_Cfg()
        {
            if (nullptr != mM_Cfg)
            {
                delete mM_Cfg;
            }
        }

        Master* LinkAndMaster_Cfg::GetMaster() { assert(nullptr != mMaster); return mMaster; }

        unsigned int LinkAndMaster_Cfg::ParseArguments(unsigned int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(nullptr != aVector);

            assert(nullptr != mConfigurator);
            assert(Link::QTY > mLink);
            assert(nullptr == mMaster);
            assert(nullptr == mM_Cfg);

            unsigned int lResult = 1;

            if (2 < aCount)
            {
                KMS::Enum<Link, LINK_NAMES> lLink(mLink);

                if (lLink.SetName_Try(aVector[1]))
                {
                    lResult = 2;

                    mLink = lLink;
                }
            }

            switch (mLink)
            {
            case Link::COM:
                mMaster = &mM_Com;
                mConfigurator->AddConfigurable(&mPort);
                break;

            case Link::TCP:
                mMaster = &mM_TCP;
                mConfigurator->AddConfigurable(mM_TCP.GetSocket());
                break;

            default: assert(false);
            }

            mM_Cfg = new Master_Cfg(mMaster);

            mConfigurator->AddConfigurable(mM_Cfg);

            return lResult;
        }

    }
}
