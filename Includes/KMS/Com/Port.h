
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Com/Port.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Enum.h>

#include <KMS/Dev/Device.h>

namespace KMS
{
    namespace Com
    {

        class Port : public Dev::Device
        {

        public:

            enum class Parity
            {
                EVEN,
                IGNORED_EVEN,
                IGNORED_MARK,
                IGNORED_ODD,
                IGNORED_SPACE,
                MARK,
                NONE,
                ODD,
                SPACE,

                QTY
            };

            static const char* PARITY_NAMES[];

            static const bool     DTR_DEFAULT;
            static const Parity   PARITY_DEFAULT;
            static const uint32_t READ_TIMEOUT_DEFAULT_ms;
            static const uint32_t READ_TIMEOUT_MAX_ms;
            static const bool     RTS_DEFAULT;
            static const uint32_t SPEED_DEFAULT_bps;
            static const uint32_t SPEED_MAX_bps;
            static const uint32_t SPEED_MIN_bps;
            static const uint32_t WRITE_TIMEOUT_DEFAULT_ms;
            static const uint32_t WRITE_TIMEOUT_MAX_ms;

            DI::Boolean                    mDTR;
            DI::Enum<Parity, PARITY_NAMES> mParity;
            DI::UInt<uint32_t>             mReadTimeout_ms;
            DI::Boolean                    mRTS;
            DI::UInt<uint32_t>             mSpeed_bps;
            DI::UInt<uint32_t>             mWriteTimeout_ms;

            Port();

            bool GetCTS() const;
            bool GetDSR() const;

            void SetDTR(bool aDTR);
            void SetRTS(bool aRTS);

            void SetParity(Parity aN);

            void SetReadTimeout(unsigned int aRT_ms);

            void SetSpeed(unsigned int aS_bps);

            void SetWriteTimeout(unsigned int aWT_ms);

            void Wait();

            // ===== Stream::IStream ========================================
            virtual bool Connect();

            // ===== Dev::IDevice ===========================================
            virtual void SetConnectFlags(unsigned int aFlags);
            virtual void ClearReadBuffer();

        // Internal

            void Display(std::ostream& aOut) const;

        protected:

            // ===== Dev::Device ============================================
            virtual void LinkFromIndex();

        private:

            NO_COPY(Port);

            void ApplyConfig  ();
            void ApplySignals ();
            void ApplyTimeouts();

            void UpdateSignals() const;

            mutable bool mCTS;
            mutable bool mDSR;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Com::Port& aP);
