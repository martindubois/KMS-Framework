
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/Container.h
// Library   KMS-A

#pragma once

namespace KMS
{
    namespace DI2
    {

        class Input;
        class IType;
        class Output;

        class Container
        {

        protected:

            Container(const IType* aElementType);

            void CodeElement_ASCII(const void* aData, Output* aOutput) const;
            void CodeElement_JSON(const void* aData, Output* aOutput) const;
            void DecodeElement_ASCII(void* aData, Input* aInput) const;
            void DecodeElement_JSON(void* aData, Input* aInput) const;

        private:

            const IType* mElementType;

        };

    }
}
