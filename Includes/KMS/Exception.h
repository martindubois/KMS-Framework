
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2022-2026 KMS
/// \file      Includes/KMS/Exception.h

#pragma once

// ===== C++ ================================================================
#include <exception>
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Result.h>
#include <KMS/String.h>

namespace KMS
{

    /// \brief Exception class
    class Exception final : public std::exception
    {

    public:

        /// \brief Register a structured exception translator
        /// \return The current translator. Pass this value to
        ///         `RestoreTranslator` to unregister the translator
        static void* RegisterTranslator();

        /// \brief Restore a translator
        /// \param aTranslator Usualy the value `RegisterTranslater` returned
        static void RestoreTranslator(void* aTranslator);

        /// \param aFile     The source file name
        /// \param aFunction The function name
        /// \param aLine     The source code line number
        /// \param aCode     See Result
        /// \param aMsg      Error message
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg);

        /// \param aFile     The source file name
        /// \param aFunction The function name
        /// \param aLine     The source code line number
        /// \param aCode     See Result
        /// \param aMsg      Error message
        /// \param aInfo     Additional information
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg, const char * aInfo);

        /// \param aFile     The source file name
        /// \param aFunction The function name
        /// \param aLine     The source code line number
        /// \param aCode     See Result
        /// \param aMsg      Error message
        /// \param aInfo     Additional information
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg, uint64_t aInfo);

        /// \return The exception code
        Result GetCode() const;

        /// \return The source file name
        const char * GetFile() const;

        /// \return The function name
        const char * GetFunction() const;

        /// \return The additional information
        const char * GetInfo () const;

        /// \return The recorder last error code
        unsigned int GetLastError() const;

        /// \return The source line number
        unsigned int GetLine() const;

        // ===== std::exception =============================================
        virtual const char* what() const throw();

    // Internal

        void Display(std::ostream& aOut) const;

    private:

        void Construct(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg);

        void Construct_OSDep();

        Result mCode;

        String_ASCII mFile;
        String_ASCII mFunction;

        unsigned int mLine;

        String_ASCII mInfo;

        unsigned int mLastError;

        String_ASCII mMessage;

    };

}

// ===== Includes ===========================================================
// The KMS/Dbg/Log.h header file use the Exception class.
#include <KMS/Dbg/Log.h>

/// \brief Stream operator
std::ostream& operator << (std::ostream& aOut, const KMS::Exception& aE);

/// \brief Standard catch bloc
#define KMS_CATCH                                                          \
    catch (KMS::Exception eE)                                              \
    {                                                                      \
        KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);           \
    }                                                                      \
    catch (std::exception eE)                                              \
    {                                                                      \
        KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);           \
        KMS::Dbg::gLog.WriteMessage(eE.what());                            \
    }                                                                      \
    catch (...)                                                            \
    {                                                                      \
        KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);           \
        KMS::Dbg::gLog.WriteMessage("Unknown exception");                  \
    }

/// \brief Standard catch bloc storing result in a variable
#define KMS_CATCH_RESULT(R)                                                \
    catch (KMS::Exception eE)                                              \
    {                                                                      \
        KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);           \
        R = static_cast<int>(eE.GetCode());                                \
    }                                                                      \
    catch (std::exception eE)                                              \
    {                                                                      \
        KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);           \
        KMS::Dbg::gLog.WriteMessage(eE.what());                            \
        R = __LINE__;                                                      \
    }                                                                      \
    catch (...)                                                            \
    {                                                                      \
        KMS_DBG_LOG_ERROR_F(KMS::Dbg::Log::FLAG_USER_REDUNDANT);           \
        KMS::Dbg::gLog.WriteMessage("Unknown exception");                  \
        R = __LINE__;                                                      \
    }

/// \brief Throw an exception
#define KMS_EXCEPTION(C, M, I) \
    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, (C), (M), (I))

/// \brief Throw an exception if a condition is false
#define KMS_EXCEPTION_ASSERT(A, C, M, I) \
    if (!(A))                            \
    {                                    \
        KMS_EXCEPTION(C, (M), (I));      \
    }
