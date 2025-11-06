
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Struct.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Struct.h>

namespace KMS
{
    namespace DI2
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Struct::Struct(unsigned int aSize_byte) : Sized(aSize_byte) {}

        void Struct::AddField(const char* aName, unsigned int aOffset_byte, const IType* aType)
        {
            assert(nullptr != aName);
            assert(nullptr != aType);

            assert(GetSize() >= aOffset_byte + aType->GetSize());

            mFields.insert(FieldList::value_type(aName, Field(aOffset_byte, aType)));
        }

        // ===== IType ======================================================

        // { Field0 = Value0; Field1 = Value1; }
        void Struct::Code_ASCII(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        void Struct::Code_JSON(const void* aData, char* aOut, unsigned int aOutSize_byte, char** aNext) const
        {
            // TODO
        }

        // Field0 op Value0
        // .Field0 op Value0
        // { Field0 op Value0; Field1 op Value0; }
        // = { Field0 op Value0; Field1 op Value0; }
        void Struct::Decode_ASCII(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

        void Struct::Decode_JSON(void* aData, const char* aIn, const char** aNext) const
        {
            // TODO
        }

    }
}
