
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Stream_Stream_Cfg.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <winsock2.h>

// ===== Includes ===========================================================
#include <KMS/Stream/Stream_Cfg.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const char* STREAM_TYPE_NAMES[] = { "COM", "TCP" };

namespace KMS
{
    namespace Stream
    {
 
        // Public
        // //////////////////////////////////////////////////////////////////

        Stream_Cfg::Stream_Cfg(Cfg::Configurator* aConfigurator, StreamType aDefault)
            : mConfigurator(aConfigurator)
            , mSocket(Net::Socket::Type::STREAM)
            , mStream(nullptr)
            , mType(aDefault)
        {
            assert(nullptr != aConfigurator);
            assert(StreamType::QTY > aDefault);

            mSocket.mLocalAddress.Set("0.0.0.0");
        }

        Stream_Cfg::~Stream_Cfg() {}

        IStream* Stream_Cfg::GetStream() { return mStream; }

        unsigned int Stream_Cfg::ParseArguments(unsigned int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(nullptr != aVector);

            assert(nullptr != mConfigurator);
            assert(nullptr == mStream);
            assert(StreamType::QTY > mType);

            unsigned int lResult = 1;

            if (2 < aCount)
            {
                KMS::Enum<StreamType, STREAM_TYPE_NAMES> lType(mType);

                if (lType.SetName_Try(aVector[1]))
                {
                    lResult = 2;

                    mType = lType;
                }
            }

            switch (mType)
            {
            case StreamType::COM:
                mStream = &mPort;
                mConfigurator->AddConfigurable(&mPort);
                break;

            case StreamType::TCP:
                mStream = &mSocket;
                mConfigurator->AddConfigurable(&mSocket);
                break;

            default: assert(false);
            }

            return lResult;
        }

    }
}
