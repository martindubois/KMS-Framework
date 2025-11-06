
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Array.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/Sized.h>

namespace KMS
{
    namespace DI2
    {

        class Array : public Sized
        {

        public:

            Array(unsigned int aCount, const IType* aElementType);

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext = nullptr) const override;
            virtual void Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext = nullptr) const override;
            virtual void Decode_ASCII(void* aData, const char* aIn, const char** aNext = nullptr) const override;
            virtual void Decode_JSON(void* aData, const char* aIn, const char** aNext = nullptr) const override;

        private:

            const IType* mElementType;

        };

    }
}
