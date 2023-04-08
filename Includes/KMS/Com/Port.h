
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

            Port();

            bool GetCTS() const;
            bool GetDSR() const;

            void SetDTR(bool aDTR);
            void SetRTS(bool aRTS);

            void SetParity(Parity aN);

            void SetReadTimeout(unsigned int aRT_ms);

            void SetSpeed(unsigned int aS_bps);

            void SetWriteTimeout(unsigned int aWT_ms);

            void ClearReadBuffer();

            void Wait();

            // ===== Dev::Device ============================================
            virtual void Connect(unsigned int aFlags);

        // Internal

            void Display(std::ostream& aOut) const;

            // ===== Configurable attributes ================================
            DI::Boolean                    mDTR;
            DI::Enum<Parity, PARITY_NAMES> mParity;
            DI::UInt<uint32_t>             mReadTimeout_ms;
            DI::Boolean                    mRTS;
            DI::UInt<uint32_t>             mSpeed_bps;
            DI::UInt<uint32_t>             mWriteTimeout_ms;

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
