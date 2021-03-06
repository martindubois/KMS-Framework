
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Com/Port.h

#pragma once

// ===== Includes ===========================================================
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
            };

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

            // ===== Dev::Device ============================================
            virtual void Connect(unsigned int aFlags);

            // ===== Cfg::Configurable ======================================
            virtual void DisplayHelp(FILE* aOut) const;
            virtual bool SetAttribute(const char* aN, const char* aV);

        protected:

            // ===== Dev::Device ============================================
            virtual void LinkFromIndex();

        private:

            Port(const Port&);

            const Port& operator = (const Port&);

            void ApplyConfig  ();
            void ApplyTimeouts();

            // ===== Configurable attributes ================================
            Parity       mParity;
            unsigned int mReadTimeout_ms;
            unsigned int mSpeed_bps;
            unsigned int mWriteTimeout_ms;

        };

    }
}
