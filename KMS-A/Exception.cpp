
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Exception.cpp

#include "Component.h"

// ===== C ==================================================================
#include <stdint.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Enum.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg)
    {
        Construct(aFile, aFunction, aLine, aCode, aMsg);

        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
        Dbg::gLog.WriteException(*this);
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg, const char* aInfo)
    {
        Construct(aFile, aFunction, aLine, aCode, aMsg);

        mInfo = aInfo;

        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
        Dbg::gLog.WriteException(*this);
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg, uint64_t aInfo)
    {
        Construct(aFile, aFunction, aLine, aCode, aMsg);

        char lInfo[64];

        #ifdef _KMS_WINDOWS_
            sprintf_s(lInfo, "%llu 0x%llx", aInfo, aInfo);
        #endif

        #if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUS_ )
            sprintf_s(lInfo, "%lu 0x%lx", aInfo, aInfo);
        #endif

        mInfo = lInfo;

        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
        Dbg::gLog.WriteException(*this);
    }

    Result          Exception::GetCode     () const { return mCode; }
    const char    * Exception::GetFile     () const { return mFile    .c_str(); }
    const char    * Exception::GetFunction () const { return mFunction.c_str(); }
    const char    * Exception::GetInfo     () const { return mInfo    .c_str(); }
    unsigned int    Exception::GetLastError() const { return mLastError; }
    unsigned int    Exception::GetLine     () const { return mLine; }

    // ===== std::exception =============================================
    const char* Exception::what() const throw () { return mMessage.c_str(); }

    // Internal
    // //////////////////////////////////////////////////////////////////////

    void Exception::Display(std::ostream& aOut) const
    {
        aOut << "    " << what() << "\n";
        aOut << "    Code     : " << mCode     << "\n";
        aOut << "    File     : " << mFile     << " (" << mLine << ")\n";
        aOut << "    Function : " << mFunction << "\n";

        if (0 < mInfo.size())
        {
            aOut << "    Info     : " << mInfo << "\n";
        }

        if (0 != mLastError)
        {
            char lLastErrorHex[16];

            sprintf_s(lLastErrorHex, " (0x%08x)", mLastError);

            aOut << "    Last err.: " << mLastError << lLastErrorHex << "\n";
        }

        aOut << std::endl;
    }

    // Private
    // //////////////////////////////////////////////////////////////////////

    void Exception::Construct(const char* aFile, const char* aFunction, unsigned int aLine, Result aCode, const char* aMsg)
    {
        assert(nullptr != aFile);
        assert(nullptr != aFunction);
        assert(0 < aLine);
        assert(nullptr != aMsg);

        mCode      = aCode;
        mFile      = aFile;
        mFunction  = aFunction;
        mLine      = aLine;
        mMessage   = aMsg;

        #ifdef _KMS_WINDOWS_
            mLastError = ::GetLastError();
        #endif

        #if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
            mLastError = errno;
        #endif
    }

}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const Exception& aE)
{
    aE.Display(aOut);

    return aOut;
}
