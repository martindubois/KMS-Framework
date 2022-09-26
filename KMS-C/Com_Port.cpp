
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Com_Port.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>

#include <KMS/Com/Port.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_DTR              (false)
#define DEFAULT_PARITY           (Parity::NONE)
#define DEFAULT_READ_TIMEOUT_ms  (1000)
#define DEFAULT_RTS              (false)
#define DEFAULT_SPEED_bps        (115200)
#define DEFAULT_WRITE_TIMEOUT_ms (1000)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI::MetaData MD_DTR          ("DTR"         , "DTR = false | true");
static const KMS::DI::MetaData MD_PARITY       ("Parity"      , "Parity = EVEN | INORED_EVEN | IGNORED_MARK | IGNORED_ODD | IGNORED_SPACE | MARK | NONE | ODD | SPACE");
static const KMS::DI::MetaData MD_READ_TIMEOUT ("ReadTimeout" , "ReadTimeout = {ms}");
static const KMS::DI::MetaData MD_RTS          ("RTS"         , "DTR = false | true");
static const KMS::DI::MetaData MD_SPEED        ("Speed"       , "Speed = {bps}");
static const KMS::DI::MetaData MD_WRITE_TIMEOUT("WriteTimeout", "WriteTimeout = {ms}");

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static KMS::Com::Port::Parity ToParity(const char* aIn);

namespace KMS
{
    namespace Com
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Port::PARITY_NAMES[] =
        {
            "EVEN",
            "IGNORED_EVEN",
            "IGNORED_MARK",
            "IGNORED_ODD",
            "IGNORED_SPACE",
            "MARK",
            "NONE",
            "ODD",
            "SPACE",
        };

        Port::Port()
            : mDTR            (false, &MD_DTR)
            , mParity         (DEFAULT_PARITY, &MD_PARITY)
            , mRTS            (false, &MD_RTS)
            , mReadTimeout_ms (DEFAULT_READ_TIMEOUT_ms, &MD_READ_TIMEOUT)
            , mSpeed_bps      (DEFAULT_SPEED_bps, &MD_SPEED)
            , mWriteTimeout_ms(DEFAULT_WRITE_TIMEOUT_ms, &MD_WRITE_TIMEOUT)
        {
            AddEntry(&mDTR);
            AddEntry(&mParity);
            AddEntry(&mRTS);
            AddEntry(&mReadTimeout_ms);
            AddEntry(&mSpeed_bps);
            AddEntry(&mWriteTimeout_ms);
        }

        void Port::SetDTR(bool aDTR) { mDTR = aDTR; if (IsConnected()) { ApplySignals(); } }
        void Port::SetRTS(bool aRTS) { mRTS = aRTS; if (IsConnected()) { ApplySignals(); } }

        void Port::SetParity      (Parity       aP    ) { mParity          = aP    ; if (IsConnected()) { ApplyConfig  (); } }
        void Port::SetReadTimeout (unsigned int aRT_ms) { mReadTimeout_ms  = aRT_ms; if (IsConnected()) { ApplyTimeouts(); } }
        void Port::SetSpeed       (unsigned int aS_bps) { mSpeed_bps       = aS_bps; if (IsConnected()) { ApplyConfig  (); } }
        void Port::SetWriteTimeout(unsigned int aWT_ms) { mWriteTimeout_ms = aWT_ms; if (IsConnected()) { ApplyTimeouts(); } }

        // ===== Dev::Device ================================================

        void Port::Connect(unsigned int aFlags)
        {
            Dev::Device::Connect(aFlags);

            ApplyConfig();
            ApplySignals();
            ApplyTimeouts();
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
