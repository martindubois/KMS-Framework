
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Msg_Destination.cpp

// TEST COVERAGE 2022-08-15 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dbg/Log.h>
#include <KMS/Msg/IReceiver.h>

#include <KMS/Msg/Destination.h>

namespace KMS
{
    namespace Msg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Destination::Destination() { Clear(); }

        Destination::Destination(IReceiver* aR, unsigned int aCode) { Set(aR, aCode); }

        void Destination::Clear() { mCode = 0; mReceiver = NULL; }

        bool Destination::IsSet() const { return NULL != mReceiver; }

        void Destination::Set(IReceiver* aR, unsigned int aCode)
        {
            assert(NULL != aR);
            assert(0 < aCode);

            mCode = aCode;
            mReceiver = aR;
        }

        unsigned int Destination::Send(void* aSender, void* aData)
        {
            unsigned int lResult = 0;

            if (NULL != mReceiver)
            {
                assert(0 < mCode);

                try
                {
                    lResult = mReceiver->Receive(aSender, mCode, aData);

                    if (0 != (lResult & IReceiver::MSG_ACTION_CLEAR))
                    {
                        Clear();
                        lResult &= ~IReceiver::MSG_ACTION_CLEAR;
                    }
                }
                catch (Exception eE)
                {
                    KMS_DBG_LOG_ERROR();
                    Dbg::gLog.WriteException(eE);
                    lResult = IReceiver::MSG_EXCEPTION;
                }
                catch (std::exception eE)
                {
                    KMS_DBG_LOG_ERROR();
                    Dbg::gLog.WriteMessage(eE.what());
                    lResult = IReceiver::MSG_EXCEPTION_STD;
                }
                catch (...)
                {
                    KMS_DBG_LOG_ERROR();
                    lResult = IReceiver::MSG_EXCEPTION_UNKNOWN;
                }
            }
            else
            {
                KMS_DBG_LOG_INFO();
                lResult = IReceiver::MSG_IGNORED;
            }

            return lResult;
        }

    }
}
