
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI2_Container.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

#include <KMS/DI2/Container.h>

namespace KMS
{
    namespace DI2
    {

        Container::Container(const IType* aElementType) : mElementType(aElementType)
        {
            assert(nullptr != aElementType);
        }

        void Container::CodeElement_ASCII(const void* aData, Output* aOutput) const
        {
            assert(nullptr != mElementType);

            mElementType->Code_ASCII(aData, aOutput);
        }

        void Container::CodeElement_JSON(const void* aData, Output* aOutput) const
        {
            assert(nullptr != mElementType);

            mElementType->Code_JSON(aData, aOutput);
        }

        void Container::DecodeElement_ASCII(void* aData, Input* aInput) const
        {
            assert(nullptr != mElementType);

            mElementType->Decode_ASCII(aData, aInput);
        }

        void Container::DecodeElement_JSON(void* aData, Input* aInput) const
        {
            assert(nullptr != mElementType);

            mElementType->Decode_JSON(aData, aInput);
        }

    }
}
