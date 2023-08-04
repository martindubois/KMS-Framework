
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Msg_Destination.cpp

// TEST COVERAGE  2023-07-28  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>

#ifndef _KMS_EMBEDDED_
    #include <KMS/Dbg/Log.h>
#endif

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
            // assert(NULL != aR);
            // assert(0 < aCode);

            mCode = aCode;
            mReceiver = aR;
        }

        unsigned int Destination::Send(void* aSender, void* aData)
        {
            unsigned int lResult = 0;

            if (NULL != mReceiver)
            {
                // assert(0 < mCode);

                #ifndef _KMS_EMBEDDED_
                    try
                    {
                #endif

                        lResult = mReceiver->Receive(aSender, mCode, aData);

                        if (0 != (lResult & IReceiver::MSG_ACTION_CLEAR))
                        {
                            Clear();
                            lResult &= ~IReceiver::MSG_ACTION_CLEAR;
                        }

                #ifndef _KMS_EMBEDDED_
                    }
                    catch (Exception eE)
                    {
                        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
                        lResult = IReceiver::MSG_EXCEPTION;
                    }
                    catch (std::exception eE)
                    {
                        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
                        Dbg::gLog.WriteMessage(eE.what());
                        lResult = IReceiver::MSG_EXCEPTION_STD;
                    }
                    catch (...)
                    {
                        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
                        Dbg::gLog.WriteMessage("Unknown exception");
                        lResult = IReceiver::MSG_EXCEPTION_UNKNOWN;
                    }
                #endif
            }
            else
            {
                #ifndef _KMS_EMBEDDED_
                    KMS_DBG_LOG_NOISE();
                #endif

                lResult = IReceiver::MSG_IGNORED;
            }

            return lResult;
        }

    }
}
