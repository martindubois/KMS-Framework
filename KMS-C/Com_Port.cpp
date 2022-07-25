
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Com_Port.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

#include <KMS/Com/Port.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_PARITY           (Parity::NONE)
#define DEFAULT_READ_TIMEOUT_ms  (1000)
#define DEFAULT_SPEED_bps        (115200)
#define DEFAULT_WRITE_TIMEOUT_ms (1000)

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static KMS::Com::Port::Parity ToParity(const char* aIn);

namespace KMS
{
    namespace Com
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Port::Port()
            : mParity         (DEFAULT_PARITY)
            , mReadTimeout_ms (DEFAULT_READ_TIMEOUT_ms)
            , mSpeed_bps      (DEFAULT_SPEED_bps)
            , mWriteTimeout_ms(DEFAULT_WRITE_TIMEOUT_ms)
        {}

        void Port::SetParity      (Parity       aP    ) { mParity          = aP    ; if (IsConnected()) { ApplyConfig  (); } }
        void Port::SetReadTimeout (unsigned int aRT_ms) { mReadTimeout_ms  = aRT_ms; if (IsConnected()) { ApplyTimeouts(); } }
        void Port::SetSpeed       (unsigned int aS_bps) { mSpeed_bps       = aS_bps; if (IsConnected()) { ApplyConfig  (); } }
        void Port::SetWriteTimeout(unsigned int aWT_ms) { mWriteTimeout_ms = aWT_ms; if (IsConnected()) { ApplyTimeouts(); } }

        // ===== Dev::Device ================================================

        void Port::Connect(unsigned int aFlags)
        {
            Dev::Device::Connect(aFlags);

            ApplyConfig();
            ApplyTimeouts();
        }

        // ===== Cfg::Configurable ==========================================

        void Port::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "===== KMS::Com::Port =====\n"
                "Parity\n"
                "    Set the parity to the default value\n"
                "    Default: NONE\n"
                "Parity = EVEN|IGNORED_EVEN|IGNORED_MARK|IGNORED_ODD|IGNORED_SPACE|MARK|NONE|ODD|SPACE\n"
                "    Parity\n"
                "ReadTimeout\n"
                "    Set the receive timeout to the default value\n"
                "    Default: %u ms\n"
                "ReadTimeout = {Value_ms}\n"
                "    Read timeout in ms\n"
                "Speed\n"
                "    Set the speed to the default value\n"
                "    Default: %u bps\n"
                "Speed = {Value_bps}"
                "    Speed in bps\n"
                "WriteTimeout\n"
                "    Set the transmit timeout to the default value\n"
                "    Default: %u ms\n"
                "WriteTimeout = {Value_ms}\n"
                "    Transmit timeout in ms\n",
                DEFAULT_READ_TIMEOUT_ms,
                DEFAULT_SPEED_bps,
                DEFAULT_WRITE_TIMEOUT_ms);

            Dev::Device::DisplayHelp(aOut);
        }

        bool Port::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Parity"      ) { SetParity      (DEFAULT_PARITY          ); return true; }
                CFG_IF("ReadTimeout" ) { SetReadTimeout (DEFAULT_READ_TIMEOUT_ms ); return true; }
                CFG_IF("Speed"       ) { SetSpeed       (DEFAULT_SPEED_bps       ); return true; }
                CFG_IF("WriteTimeout") { SetWriteTimeout(DEFAULT_WRITE_TIMEOUT_ms); return true; }
            }
            else
            {
                CFG_CONVERT("Parity"      , SetParity      , ToParity         );
                CFG_CONVERT("ReadTimeout" , SetReadTimeout , Convert::ToUInt32);
                CFG_CONVERT("Speed"       , SetSpeed       , Convert::ToUInt32);
                CFG_CONVERT("WriteTimeout", SetWriteTimeout, Convert::ToUInt32);
            }

            return Dev::Device::SetAttribute(aA, aV);
        }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

KMS::Com::Port::Parity ToParity(const char* aIn)
{
    assert(NULL != aIn);

    if (strcmp("EVEN"        , aIn)) { return KMS::Com::Port::Parity::EVEN        ; }
    if (strcmp("IGNORED_EVEN", aIn)) { return KMS::Com::Port::Parity::IGNORED_EVEN; }
    if (strcmp("IGNORED_ODD" , aIn)) { return KMS::Com::Port::Parity::IGNORED_ODD ; }
    if (strcmp("NONE"        , aIn)) { return KMS::Com::Port::Parity::NONE        ; }
    if (strcmp("ODD"         , aIn)) { return KMS::Com::Port::Parity::ODD         ; }

    KMS_EXCEPTION_WITH_INFO(CONFIG_VALUE, "Invalid parity", aIn);
}
