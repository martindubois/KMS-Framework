
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2025-2026 KMS
/// \file      Includes/KMS/String_DI2.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

namespace KMS
{

    /// \brief The `DI2` type description for `String_ASCII`
    class String_ASCII_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output * aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    /// \brief The `DI2` type description for `String_UTF16`
    class String_UTF16_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    /// \brief The `DI2` type description for `StringList_ASCII`
    class StringList_ASCII_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    /// \brief The `DI2` type description for `StringList_UTF16`
    class StringList_UTF16_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    /// \brief The `DI2` type description for `StringSet_ASCII`
    class StringSet_ASCII_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    /// \brief The `DI2` type description for `StringSet_UTF16`
    class StringSet_UTF16_Type : public DI2::IType
    {

    public:

        // ===== IType ==================================================
        virtual void Code_ASCII(const void* aData, DI2::Output* aOutput) const override;
        virtual void Code_JSON(const void* aData, DI2::Output* aOutput) const override;
        virtual void Decode_ASCII(void* aData, DI2::Input* aInput) const override;
        virtual void Decode_JSON(void* aData, DI2::Input* aInput) const override;

    };

    /// \brief The `DI2` type description for `String_ASCII`
    const String_ASCII_Type String_ASCII_TYPE;

    /// \brief The `DI2` type description for `String_UTF16`
    const String_UTF16_Type String_UTF16_TYPE;

    /// \brief The `DI2` type description for `StringList_ASCII`
    const StringList_ASCII_Type StringList_ASCII_TYPE;

    /// \brief The `DI2` type description for `StringList_UTF16`
    const StringList_UTF16_Type StringList_UTF16_TYPE;

    /// \brief The `DI2` type description for `StringSet_ASCII`
    const StringSet_ASCII_Type StringSet_ASCII_TYPE;

    /// \brief The `DI2` type description for `StringSet_UTF16`
    const StringSet_UTF16_Type StringSet_UTF16_TYPE;

}
