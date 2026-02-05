
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2023-2026 KMS
/// \file      Includes/KMS/Result.h

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

    /// \brief Result
    /// \note This is what, here at KMS, we call a value class.
    ///       - copy allowed
    ///       - limited size
    ///       - no virtual method
    class Result final
    {

    public:

        /// \brief Result level
        enum class Level
        {
            LEVEL_SUCCESS = 0,
            LEVEL_WARNING,
            LEVEL_ERROR,

            QTY
        };

        /// \brief Result type
        enum class Type
        {
            TYPE_GLOBAL = 0,
            TYPE_MEMBER,
            TYPE_STATIC,

            QTY
        };

        /// \brief Default constructor
        Result();

        /// \brief Cast operator
        operator uint32_t () const;

        /// \retval false Different
        /// \retval true  Equal
        bool operator == (Result aIn) const;

        /// \retval false Not an error (success or warning)
        /// \retval true  An error
        bool IsError() const;

        /// \retval false A success
        /// \retval true  A warning or an error
        bool IsErrorOrWarning() const;

        /// \retval false A warning or an error
        /// \retval true  A success
        bool IsSuccess() const;

        /// \retval false An error
        /// \retval true  A warning or a success
        bool IsSuccessOrWarning() const;

        /// \retval false A success or an error
        /// \retval true  A warning
        bool IsWarning() const;

        /// \return Result's index
        unsigned int GetIndex() const;

        /// \return The level
        Level GetLevel() const;

        /// \return The result name
        const char* GetName() const;

        Type GetType() const;

    // Internal

        /// \brief Constructor
        /// \param aIn Value
        Result(uint32_t aIn);

        /// \brief Construction
        /// \param aName  Result name
        /// \param aLevel Level
        /// \param aType  Type
        /// \note Thread C++ init
        Result(const char* aName, Level aLevel, Type aType);

        /// \brief Construction
        /// \param aName  Result name
        /// \param aLevel Level
        /// \param aType  Type
        /// \param aIndex Index
        Result(const char* aName, Level aLevel, Type aType, unsigned int aIndex);

        /// \brief Display the result
        /// \param aOut The output stream
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

/// \brief Output operator
/// \param aOut  The output stream
/// \param aCode The code to display
std::ostream& operator << (std::ostream& aOut, KMS::Result aCode);

// TODO Rename KMS_RESULT_GLOBAL -> KMS_RESULT_ERROR_GLOBAL,
//      KMS_RESULT_MEMBER -> KMS_RESULT_ERROR_MEMBER, KMS_RESULT_STATIC ->
//      KMS_RESULT_ERROR_STATIC

/// \brief Create a global error code
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_GLOBAL(N) const KMS::Result N(#N, KMS::Result::Level::LEVEL_ERROR, KMS::Result::Type::TYPE_GLOBAL)

/// \brief Create a member error code
/// \param C The class
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_MEMBER(C, N) static const KMS::Result C::N(#N, KMS::Result::Level::LEVEL_ERROR, KMS::Result::Type::TYPE_MEMBER)

/// \brief Create a static error code
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_STATIC(N) static const KMS::Result N(#N, KMS::Result::Level::LEVEL_ERROR, KMS::Result::Type::TYPE_STATIC)

/// \brief Create a global success code
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_SUCCESS_GLOBAL(N) const KMS::Result N(#N, KMS::Result::Level::LEVEL_SUCCESS, KMS::Result::Type::TYPE_GLOBAL)

/// \brief Create a member success code
/// \param C The class
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_SUCCESS_MEMBER(C, N) static const KMS::Result C::N(#N, KMS::Result::Level::LEVEL_SUCCESS, KMS::Result::Type::TYPE_MEMBER)

/// \brief Create a static success code
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_SUCCESS_STATIC(N) static const KMS::Result N(#N, KMS::Result::Level::LEVEL_SUCCESS, KMS::Result::Type::TYPE_STATIC)

/// \brief Create a global warning code
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_WARNING_GLOBAL(N) const KMS::Result N(#N, KMS::Result::Level::LEVEL_WARNING, KMS::Result::Type::TYPE_GLOBAL)

/// \brief Create a member warning code
/// \param C The class
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_WARNING_MEMBER(C, N) static const KMS::Result C::N(#N, KMS::Result::Level::LEVEL_WARNIN, KMS::Result::Type::TYPE_MEMBER)

/// \brief Create a static warning code
/// \param N The name
/// \note Thread C++ Init
#define KMS_RESULT_WARNING_STATIC(N) static const KMS::Result N(#N, KMS::Result::Level::LEVEL_WARNING, KMS::Result::Type::TYPE_STATIC)
