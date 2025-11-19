
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/IType.h
// Library   KMS-A

#pragma once

namespace KMS
{
    namespace DI2
    {

        class Input;
        class Output;

        class IType
        {

        public:

            virtual void Code_ASCII(const void* aData, Output* aOutput) const = 0;

            virtual void Code_JSON(const void* aData, Output* aOutput) const = 0;

            virtual void Decode_ASCII(void* aData, Input* aInput) const = 0;

            virtual void Decode_JSON(void* aData, Input* aInput) const = 0;

        };

    }
}
