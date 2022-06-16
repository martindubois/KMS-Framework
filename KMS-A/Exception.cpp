
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static const char* ToCodeName(KMS::Exception::Code aCode);

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage)
    {
        Construct(aFile, aFunction, aLine, aCode, aMessage);
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage, const char* aInfo)
    {
        Construct(aFile, aFunction, aLine, aCode, aMessage);

        mInfo = aInfo;
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage, uint64_t aInfo)
    {
        Construct(aFile, aFunction, aLine, aCode, aMessage);

        char lInfo[64];

        #ifdef _KMS_WINDOWS_
            sprintf_s(lInfo, "%llu 0x%llx", aInfo, aInfo);
        #endif

        #if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUS_ )
            sprintf_s(lInfo, "%lu 0x%lx", aInfo, aInfo);
        #endif

        mInfo = lInfo;
    }

    Exception::Code Exception::GetCode     () const { return mCode; }
    const char    * Exception::GetCodeName () const { return ToCodeName(mCode); }
    const char    * Exception::GetFile     () const { return mFile    .c_str(); }
    const char    * Exception::GetFunction () const { return mFunction.c_str(); }
    const char    * Exception::GetInfo     () const { return mInfo    .c_str(); }
    unsigned int    Exception::GetLastError() const { return mLastError; }
    unsigned int    Exception::GetLine     () const { return mLine; }

    // ===== std::exception =============================================
    const char* Exception::what() const throw () { return mMessage.c_str(); }

    // Private
    // //////////////////////////////////////////////////////////////////////

    void Exception::Construct(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage)
    {
        assert(NULL != aFile);
        assert(NULL != aFunction);
        assert(0 < aLine);
        assert(Code::CODE_QTY > aCode);
        assert(NULL != aMessage);

        mCode      = aCode;
        mFile      = aFile;
        mFunction  = aFunction;
        mLine      = aLine;
        mMessage   = aMessage;

        #ifdef _KMS_WINDOWS_
            mLastError = ::GetLastError();
        #endif

        #if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
            mLastError = errno;
        #endif
    }

}

std::ostream& operator << (std::ostream& aOut, const KMS::Exception& aE)
{
    const char * lInfo      = aE.GetInfo();
    unsigned int lLastError = aE.GetLastError();

    aOut << aE.what() << std::endl;
    aOut << "Code     : " << aE.GetCode    () << std::endl;
    aOut << "File     : " << aE.GetFile    () << std::endl;
    aOut << "Function : " << aE.GetFunction() << std::endl;
    aOut << "Line     : " << aE.GetLine    () << std::endl;

    if (0 < strlen(lInfo))
    {
        aOut << "Info     : " << aE.GetInfo() << std::endl;
    }

    if (0 != lLastError)
    {
        char lLastErrorHex[16];

        sprintf_s(lLastErrorHex, " (0x%08x)", lLastError);

        aOut << "Last err.: " << lLastError << lLastErrorHex << std::endl;
    }

    return aOut;
}

std::ostream& operator << (std::ostream& aOut, const KMS::Exception::Code& aC)
{
    aOut << ToCodeName(aC);

    return aOut;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

const char* ToCodeName(KMS::Exception::Code aCode)
{
    static const char* CODE_NAMES[static_cast<unsigned int>(KMS::Exception::Code::CODE_QTY)] =
    {
        "BUILD_COMPILE", "BUILD_TEST",
        "CONFIG", "CONFIG_EXPAND", "CONFIG_FORMAT", "CONFIG_INDEX",
        "CONVERT_FORMAT", "CONVERT_TYPE",
        "DEPENDENCY",
        "FILE_ACCESS", "FILE_BACKUP", "FILE_COPY", "FILE_DELETE",
            "FILE_OPEN", "FILE_READ", "FILE_RENAME", "FILE_WRITE",
        "FOLDER", "FOLDER_ACCESS", "FOLDER_COMPRESS", "FOLDER_CREATE",
            "FOLDER_INIT", "FOLDER_REMOVE", "FOLDER_UNCOMPRESS",
        "HTTP_REQUEST",
        "MAKE_DEPEND", "MAKE_MAKE",
        "NETWORK_ADDRESS", "NETWORK_ADDRESS_RANGE", "NETWORK_PORT",
        "OUTPUT_TOO_SHORT",
        "PROCESS_EXIT_CODE", "PROCESS_START", "PROCESS_TIMEOUT",
        "SOCKET", "SOCKET_ACCEPT", "SOCKET_BIND", "SOCKET_LISTEN",
            "SOCKET_OPTION", "SOCKET_RECEIVE", "SOCKET_SEND", "SOCKET_STARTUP",
        "STATE",
        "TEST",
        "THREAD_START",
        "VERSION_FILE", "VERSION_NUMBER",
    };

    unsigned int lCode = static_cast<unsigned int>(aCode);

    assert(static_cast<unsigned int>(KMS::Exception::Code::CODE_QTY) > lCode);

    return CODE_NAMES[lCode];
}
