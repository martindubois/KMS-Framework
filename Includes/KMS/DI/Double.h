
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Double.h
// Library   KMS-A

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace DI
    {

        class Double : public Value
        {

        public:

            static DI::Object* Create();

            Double(double aIn = 0.0);

            const Double& operator = (double aIn);

            operator double () const;

            void Clear();

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Double();

        private:

            double mInternal;

        };

    }
}
