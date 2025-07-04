
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Result.h

#pragma once

// ===== C ==================================================================
#include <stdint.h>

// ===== C++ ================================================================
#include <ostream>

#define KMS_RESULT_GLOBAL_DECL(N) \
    extern const KMS::Result N

#define KMS_RESULT_MEMBER_DECL(N) \
    static const KMS::Result N

namespace KMS
{

    // Value class
    // - Limited size
    // - No virtual method
    // - Copy allowed
    class Result final
    {

    public:

        enum class Level
        {
            LEVEL_SUCCESS = 0,
            LEVEL_WARNING,
            LEVEL_ERROR,

            QTY
        };

        enum class Type
        {
            TYPE_GLOBAL = 0,
            TYPE_MEMBER,
            TYPE_STATIC,

            QTY
        };

        Result();

        operator uint32_t () const;

        bool operator == (Result aIn) const;

        bool IsError           () const;
        bool IsErrorOrWarning  () const;
        bool IsSuccess         () const;
        bool IsSuccessOrWarning() const;
        bool IsWarning         () const;

        unsigned int GetIndex() const;

        Level GetLevel() const;

        const char* GetName() const;

        unsigned int GetText(char* aOut, unsigned int aOutSize_byte) const;

        Type GetType() const;

    // Internal

        Result(uint32_t aIn);

        // Thread  C++ init
        Result(const char* aName, Level aLevel, Type aType);

        Result(const char* aName, Level aLevel, Type aType, unsigned int aIndex);

        void Display(std::ostream& aOut) const;

    private:

        union
        {
            struct Code
            {
                unsigned mLevel       :  2;
                unsigned mType        :  2;
                unsigned mReserved    :  4;
                unsigned mIndex       : 24;
            }
            mFields;
                
            uint32_t mCode;
        }
        mUnion;

    };

    KMS_RESULT_GLOBAL_DECL(RESULT_OK);
    KMS_RESULT_GLOBAL_DECL(RESULT_WARNING);
    KMS_RESULT_GLOBAL_DECL(RESULT_ERROR);

    KMS_RESULT_GLOBAL_DECL(RESULT_CONNECT_FAILED);
    KMS_RESULT_GLOBAL_DECL(RESULT_CRC_ERROR);
    KMS_RESULT_GLOBAL_DECL(RESULT_DENIED);
    KMS_RESULT_GLOBAL_DECL(RESULT_FILE_FORMAT_ERROR);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_CLASS);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_COMMAND);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_CONFIG);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_DATA_TYPE);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_FORMAT);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_INDEX);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_KEY);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_LINE_NO);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_NAME);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_STATE);
    KMS_RESULT_GLOBAL_DECL(RESULT_INVALID_VALUE);
    KMS_RESULT_GLOBAL_DECL(RESULT_NOT_IMPLEMENTED);
    KMS_RESULT_GLOBAL_DECL(RESULT_OPEN_FAILED);
    KMS_RESULT_GLOBAL_DECL(RESULT_OPERATION_FAILED);
    KMS_RESULT_GLOBAL_DECL(RESULT_OUTPUT_TOO_SHORT);
    KMS_RESULT_GLOBAL_DECL(RESULT_READ_FAILED);
    KMS_RESULT_GLOBAL_DECL(RESULT_RECEIVE_FAILED);
    KMS_RESULT_GLOBAL_DECL(RESULT_SEND_FAILED);
    KMS_RESULT_GLOBAL_DECL(RESULT_TIMEOUT);
    KMS_RESULT_GLOBAL_DECL(RESULT_WRITE_FAILED);

}

std::ostream& operator << (std::ostream& aOut, KMS::Result aCode);

// Thread  C++ Init
#define KMS_RESULT_GLOBAL(N) const KMS::Result N(#N, KMS::Result::Level::LEVEL_ERROR, KMS::Result::Type::TYPE_GLOBAL)

// Thread  C++ Init
#define KMS_RESULT_MEMBER(C, N) static const KMS::Result C::N(#N, KMS::Result::Level::LEVEL_ERROR, KMS::Result::Type::TYPE_MEMBER)

// Thread  C++ Init
#define KMS_RESULT_STATIC(N) static const KMS::Result N(#N, KMS::Result::Level::LEVEL_ERROR, KMS::Result::Type::TYPE_STATIC)

// Thread  C++ Init
#define KMS_RESULT_SUCCESS_GLOBAL(N) const KMS::Result N(#N, KMS::Result::Level::LEVEL_SUCCESS, KMS::Result::Type::TYPE_GLOBAL)

// Thread  C++ Init
#define KMS_RESULT_SUCCESS_MEMBER(C, N) static const KMS::Result C::N(#N, KMS::Result::Level::LEVEL_SUCCESS, KMS::Result::Type::TYPE_MEMBER)

// Thread  C++ Init
#define KMS_RESULT_SUCCESS_STATIC(N) static const KMS::Result N(#N, KMS::Result::Level::LEVEL_SUCCESS, KMS::Result::Type::TYPE_STATIC)

// Thread  C++ Init
#define KMS_RESULT_WARNING_GLOBAL(N) const KMS::Result N(#N, KMS::Result::Level::LEVEL_WARNING, KMS::Result::Type::TYPE_GLOBAL)

// Thread  C++ Init
#define KMS_RESULT_WARNING_MEMBER(C, N) static const KMS::Result C::N(#N, KMS::Result::Level::LEVEL_WARNIN, KMS::Result::Type::TYPE_MEMBER)

// Thread  C++ Init
#define KMS_RESULT_WARNING_STATIC(N) static const KMS::Result N(#N, KMS::Result::Level::LEVEL_WARNING, KMS::Result::Type::TYPE_STATIC)

