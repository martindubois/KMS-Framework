
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Com_Port.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Com/Port.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_DTR          ("DTR = false | true");
static const KMS::Cfg::MetaData MD_PARITY       ("Parity = EVEN | INORED_EVEN | IGNORED_MARK | IGNORED_ODD | IGNORED_SPACE | MARK | NONE | ODD | SPACE");
static const KMS::Cfg::MetaData MD_READ_TIMEOUT ("ReadTimeout = {ms}");
static const KMS::Cfg::MetaData MD_RTS          ("RTS = false | true");
static const KMS::Cfg::MetaData MD_SPEED        ("Speed = {bps}");
static const KMS::Cfg::MetaData MD_WRITE_TIMEOUT("WriteTimeout = {ms}");

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

        const bool         Port::DTR_DEFAULT              = false;
        const Port::Parity Port::PARITY_DEFAULT           = Parity::NONE;
        const uint32_t     Port::READ_TIMEOUT_DEFAULT_ms  =     1000;
        const uint32_t     Port::READ_TIMEOUT_MAX_ms      =    90000;
        const bool         Port::RTS_DEFAULT              =    false;
        const uint32_t     Port::SPEED_DEFAULT_bps        =   115200;
        const uint32_t     Port::SPEED_MAX_bps            = 10000000;
        const uint32_t     Port::SPEED_MIN_bps            =       75;
        const uint32_t     Port::WRITE_TIMEOUT_DEFAULT_ms =     1000;
        const uint32_t     Port::WRITE_TIMEOUT_MAX_ms     =    90000;

        Port::Port()
            : mDTR            (DTR_DEFAULT)
            , mParity         (PARITY_DEFAULT)
            , mReadTimeout_ms (READ_TIMEOUT_DEFAULT_ms)
            , mRTS            (RTS_DEFAULT)
            , mSpeed_bps      (SPEED_DEFAULT_bps)
            , mWriteTimeout_ms(WRITE_TIMEOUT_DEFAULT_ms)
            , mCTS(false)
            , mDSR(false)
        {
            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mDTR            , false); AddEntry("DTR"         , lEntry, &MD_DTR);
            lEntry.Set(&mParity         , false); AddEntry("Parity"      , lEntry, &MD_PARITY);
            lEntry.Set(&mRTS            , false); AddEntry("RTS"         , lEntry, &MD_RTS);
            lEntry.Set(&mReadTimeout_ms , false); AddEntry("ReadTimeout" , lEntry, &MD_READ_TIMEOUT);
            lEntry.Set(&mSpeed_bps      , false); AddEntry("Speed"       , lEntry, &MD_SPEED);
            lEntry.Set(&mWriteTimeout_ms, false); AddEntry("WriteTimeout", lEntry, &MD_WRITE_TIMEOUT);
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

        bool Port::Connect(unsigned int aFlags)
        {
            bool lResult = Dev::Device::Connect(aFlags);
            if (lResult)
            {
                ApplyConfig();
                ApplySignals();
                ApplyTimeouts();
            }

            return lResult;
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
