
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Com_Port_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Com/Port.h>

namespace KMS
{
    namespace Com
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // ===== Dev::IDevice ===============================================

        void Port::ClearReadBuffer()
        {
            auto lRetB = PurgeComm(*this, PURGE_RXCLEAR);
            KMS_EXCEPTION_ASSERT(lRetB, COM_CLEAR_FAILED, "Cannot clear the read buffer", "");
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Port::LinkFromIndex()
        {
            char lLink[16];

            sprintf_s(lLink, "COM%u", GetIndex());

            SetLink(lLink);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Port::ApplyConfig()
        {
            DCB lDCB;

            memset(&lDCB, 0, sizeof(lDCB));

            lDCB.DCBlength = sizeof(lDCB);

            if (!GetCommState(*this, &lDCB))
            {
                KMS_EXCEPTION(COM_CONFIG_FAILED, "Cannot retrieve the configuration of the port", "");
            }

            lDCB.BaudRate          = mSpeed_bps;
            lDCB.fBinary           = true;
            lDCB.fOutxCtsFlow      = false;
            lDCB.fOutxDsrFlow      = false;
            lDCB.fDtrControl       = DTR_CONTROL_DISABLE;
            lDCB.fDsrSensitivity   = false;
            lDCB.fTXContinueOnXoff = true;
            lDCB.fOutX             = false;
            lDCB.fInX              = false;
            lDCB.fErrorChar        = false;
            lDCB.fNull             = false;
            lDCB.fRtsControl       = RTS_CONTROL_DISABLE;
            lDCB.ByteSize          = 8;
            lDCB.StopBits          = ONESTOPBIT;

            switch (mParity)
            {
            case Parity::EVEN         : lDCB.fParity = true ; lDCB.Parity = EVENPARITY ; break;
            case Parity::IGNORED_EVEN : lDCB.fParity = false; lDCB.Parity = EVENPARITY ; break;
            case Parity::IGNORED_MARK : lDCB.fParity = false; lDCB.Parity = MARKPARITY ; break;
            case Parity::IGNORED_ODD  : lDCB.fParity = false; lDCB.Parity = ODDPARITY  ; break;
            case Parity::IGNORED_SPACE: lDCB.fParity = false; lDCB.Parity = SPACEPARITY; break;
            case Parity::MARK         : lDCB.fParity = true ; lDCB.Parity = MARKPARITY ; break;
            case Parity::NONE         : lDCB.fParity = false; lDCB.Parity = NOPARITY   ; break;
            case Parity::ODD          : lDCB.fParity = true ; lDCB.Parity = ODDPARITY  ; break;
            case Parity::SPACE        : lDCB.fParity = true ; lDCB.Parity = SPACEPARITY; break;

            default: assert(false);
            }

            if (!SetCommState(*this, &lDCB))
            {
                KMS_EXCEPTION(COM_CONFIG_FAILED, "Cannot set the configuration of the port", "");
            }
        }

        void Port::ApplySignals()
        {
            if (!EscapeCommFunction(*this, mDTR ? SETDTR : CLRDTR))
            {
                KMS_EXCEPTION(COM_CONTROL_FAILED, "Cannot update DTR signal", "");
            }

            if (!EscapeCommFunction(*this, mRTS ? SETRTS : CLRRTS))
            {
                KMS_EXCEPTION(COM_CONTROL_FAILED, "Cannot update RTS signal", "");
            }
        }

        void Port::ApplyTimeouts()
        {
            COMMTIMEOUTS lCT;

            lCT.ReadIntervalTimeout         = mReadTimeout_ms;
            lCT.ReadTotalTimeoutMultiplier  = 0;
            lCT.ReadTotalTimeoutConstant    = mReadTimeout_ms;
            lCT.WriteTotalTimeoutMultiplier = 0;
            lCT.WriteTotalTimeoutConstant   = mWriteTimeout_ms;

            if (!SetCommTimeouts(*this, &lCT))
            {
                KMS_EXCEPTION(COM_CONFIG_FAILED, "Cannot set the communication timeouts", "");
            }
        }

        void Port::UpdateSignals() const
        {
            DWORD lBits;

            auto lThis = const_cast<Port*>(this);

            if (!GetCommModemStatus(*lThis, &lBits))
            {
                KMS_EXCEPTION(COM_CONTROL_FAILED, "Cannot read signals", "");
            }

            mCTS = (0 != (lBits & MS_CTS_ON));
            mDSR = (0 != (lBits & MS_DSR_ON));
        }

    }
}
