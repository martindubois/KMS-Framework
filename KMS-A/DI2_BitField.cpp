
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_BitField.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/BitField.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        BitField::BitField(const IType* aType) : mType(aType)
        {
            assert(nullptr != aType);
        }

        void BitField::AddField(const char* aName, unsigned int aBits)
        {
            assert(nullptr != aName);

            mFields.insert(FieldList::value_type(aName, aBits));
        }

        // ===== IType ======================================================

        unsigned int BitField::GetSize() const
        {
            assert(nullptr != mType);

            return mType->GetSize();
        }

        // Value
        void BitField::Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            assert(nullptr != mType);

            mType->Code_ASCII(aData, aOut, aOutSize_byte, aNext);
        }

        // Value
        void BitField::Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            assert(nullptr != mType);

            mType->Code_JSON(aData, aOut, aOutSize_byte, aNext);
        }

        // Operators = += -= *= /= |= &= ^=
        //
        // Value
        // op Value
        // Field0 op Value0
        // .Field0 op Value0
        // { Field0 op Value0; Field1 op Value1; }
        // = { Field0 op Value0; Field1 op Value1; }
        void BitField::Decode_ASCII(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

        // Value
        void BitField::Decode_JSON(void* aData, const char* aIn, const char** aNext) const
        {
            assert(nullptr != mType);

            mType->Decode_JSON(aData, aIn, aNext);
        }

    }
}
