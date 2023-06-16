
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Com_Port.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

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

static const KMS::Cfg::MetaData MD_DTR          ("DTR = false | true");
static const KMS::Cfg::MetaData MD_PARITY       ("Parity = EVEN | INORED_EVEN | IGNORED_MARK | IGNORED_ODD | IGNORED_SPACE | MARK | NONE | ODD | SPACE");
static const KMS::Cfg::MetaData MD_READ_TIMEOUT ("ReadTimeout = {ms}");
static const KMS::Cfg::MetaData MD_RTS          ("DTR = false | true");
static const KMS::Cfg::MetaData MD_SPEED        ("Speed = {bps}");
static const KMS::Cfg::MetaData MD_WRITE_TIMEOUT("WriteTimeout = {ms}");

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
            : mDTR            (DEFAULT_DTR)
            , mParity         (DEFAULT_PARITY)
            , mRTS            (DEFAULT_RTS)
            , mReadTimeout_ms (DEFAULT_READ_TIMEOUT_ms)
            , mSpeed_bps      (DEFAULT_SPEED_bps)
            , mWriteTimeout_ms(DEFAULT_WRITE_TIMEOUT_ms)
            , mCTS(false)
            , mDSR(false)
        {
            AddEntry("DTR"         , &mDTR            , false, &MD_DTR);
            AddEntry("Parity"      , &mParity         , false, &MD_PARITY);
            AddEntry("RTS"         , &mRTS            , false, &MD_RTS);
            AddEntry("ReadTimeout" , &mReadTimeout_ms , false, &MD_READ_TIMEOUT);
            AddEntry("Speed"       , &mSpeed_bps      , false, &MD_SPEED);
            AddEntry("WriteTimeout", &mWriteTimeout_ms, false, &MD_WRITE_TIMEOUT);
        }

        bool Port::GetCTS() const { if (IsConnected()) { UpdateSignals(); } return mCTS; }
        bool Port::GetDSR() const { if (IsConnected()) { UpdateSignals(); } return mDSR; }

        void Port::SetDTR(bool aDTR) { mDTR = aDTR; if (IsConnected()) { ApplySignals(); } }
        void Port::SetRTS(bool aRTS) { mRTS = aRTS; if (IsConnected()) { ApplySignals(); } }

        void Port::SetParity      (Parity       aP    ) { mParity          = aP    ; if (IsConnected()) { ApplyConfig  (); } }
        void Port::SetReadTimeout (unsigned int aRT_ms) { mReadTimeout_ms  = aRT_ms; if (IsConnected()) { ApplyTimeouts(); } }
        void Port::SetSpeed       (unsigned int aS_bps) { mSpeed_bps       = aS_bps; if (IsConnected()) { ApplyConfig  (); } }
        void Port::SetWriteTimeout(unsigned int aWT_ms) { mWriteTimeout_ms = aWT_ms; if (IsConnected()) { ApplyTimeouts(); } }

        // ===== Dev::IDevice ================================================

        void Port::Connect(unsigned int aFlags)
        {
            Dev::Device::Connect(aFlags);

            ApplyConfig();
            ApplySignals();
            ApplyTimeouts();
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Port::Display(std::ostream& aOut) const
        {
            if (IsConnected())
            {
                aOut << "Connected\n";
            }
            else
            {
                aOut << "Not connected\n";
            }

            aOut << "Link         : " << mLink << "\n";
            aOut << "Speed        : " << mSpeed_bps << " bps\n";
            aOut << "Parity       : " << mParity << "\n";
            aOut << "ReadTimeout  : " << mReadTimeout_ms << " ms\n";
            aOut << "WriteTimeout : " << mWriteTimeout_ms << " ms\n";

            aOut << "CTS          : " << (GetCTS() ? "1" : "0") << "\n";
            aOut << "DSR          : " << (GetDSR() ? "1" : "0") << "\n";
            aOut << "DTR          : " << mDTR << "\n";
            aOut << "RTS          : " << mRTS << "\n";
        }

    }
}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const Com::Port& aP)
{
    aP.Display(aOut);

    return aOut;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

Com::Port::Parity ToParity(const char* aIn)
{
    assert(NULL != aIn);

    if (strcmp("EVEN"        , aIn)) { return Com::Port::Parity::EVEN        ; }
    if (strcmp("IGNORED_EVEN", aIn)) { return Com::Port::Parity::IGNORED_EVEN; }
    if (strcmp("IGNORED_ODD" , aIn)) { return Com::Port::Parity::IGNORED_ODD ; }
    if (strcmp("NONE"        , aIn)) { return Com::Port::Parity::NONE        ; }
    if (strcmp("ODD"         , aIn)) { return Com::Port::Parity::ODD         ; }

    KMS_EXCEPTION(COM_CONFIG_INVALID, "Invalid parity", aIn);
}
