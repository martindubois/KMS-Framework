
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/String_DI2.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

namespace KMS
{

    class String_ASCII_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output * aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    class String_UTF16_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    class StringList_ASCII_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    class StringList_UTF16_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    class StringSet_ASCII_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    class StringSet_UTF16_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    const String_ASCII_Type String_ASCII_TYPE;
    const String_UTF16_Type String_UTF16_TYPE;

    const StringList_ASCII_Type StringList_ASCII_TYPE;
    const StringList_UTF16_Type StringList_UTF16_TYPE;

    const StringSet_ASCII_Type StringSet_ASCII_TYPE;
    const StringSet_UTF16_Type StringSet_UTF16_TYPE;

}
