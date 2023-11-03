
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Exception.h

#pragma once

// ===== C++ ================================================================
#include <exception>
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Result.h>

namespace KMS
{

    class Exception final : public std::exception
    {

    public:

        static void* RegisterTranslator();

        static void RestoreTranslator(void* aTranslator);

        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg);
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg, const char * aInfo);
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg, uint64_t aInfo);

        Result       GetCode     () const;
        const char * GetFile     () const;
        const char * GetFunction () const;
        const char * GetInfo     () const;
        unsigned int GetLastError() const;
        unsigned int GetLine     () const;

        // ===== std::exception =============================================
        virtual const char* what() const throw();

    // Internal

        void Display(std::ostream& aOut) const;

    private:

        void Construct(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg);

        void Construct_OSDep();

        Result mCode;

        std::string mFile;
        std::string mFunction;

        unsigned int mLine;

        std::string mInfo;

        unsigned int mLastError;

        std::string mMessage;

    };

}

// ===== Includes ===========================================================
// The KMS/Dbg/Log.h header file use the Exception class.
#include <KMS/Dbg/Log.h>

std::ostream& operator << (std::ostream& aOut, const KMS::Exception& aE);

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

#define KMS_EXCEPTION(C, M, I) \
    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, (C), (M), (I))

#define KMS_EXCEPTION_ASSERT(A, C, M, I) \
    if (!(A))                            \
    {                                    \
        KMS_EXCEPTION(C, (M), (I));      \
    }
