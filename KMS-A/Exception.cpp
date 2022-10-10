
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

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg)
    {
        Construct(aFile, aFunction, aLine, aCode, aMsg);

        KMS_DBG_LOG_ERROR();
        Dbg::gLog.WriteException(*this);
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg, const char* aInfo)
    {
        Construct(aFile, aFunction, aLine, aCode, aMsg);

        mInfo = aInfo;

        KMS_DBG_LOG_ERROR();
        Dbg::gLog.WriteException(*this);
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg, uint64_t aInfo)
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

        KMS_DBG_LOG_ERROR();
        Dbg::gLog.WriteException(*this);
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

    void Exception::Construct(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg)
    {
        assert(NULL != aFile);
        assert(NULL != aFunction);
        assert(0 < aLine);
        assert(Code::CODE_QTY > aCode);
        assert(NULL != aMsg);

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

std::ostream& operator << (std::ostream& aOut, const KMS::Exception& aE)
{
    const char * lInfo      = aE.GetInfo();
    unsigned int lLastError = aE.GetLastError();

    aOut << aE.what() << "\n";
    aOut << "Code     : " << aE.GetCode    () << "\n";
    aOut << "File     : " << aE.GetFile    () << "\n";
    aOut << "Function : " << aE.GetFunction() << "\n";
    aOut << "Line     : " << aE.GetLine    () << "\n";

    if (0 < strlen(lInfo))
    {
        aOut << "Info     : " << aE.GetInfo() << "\n";
    }

    if (0 != lLastError)
    {
        char lLastErrorHex[16];

        sprintf_s(lLastErrorHex, " (0x%08x)", lLastError);

        aOut << "Last err.: " << lLastError << lLastErrorHex << "\n";
    }

    aOut << std::flush;

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
        "APPLICATION",
        "ARGUMENT",
        "BROWSER_START",
        "BUILD_COMMAND", "BUILD_COMPILE", "BUILD_TEST",
        "COM_CONFIG", "COM_CONNECT", "COM_CONTROL",
        "CONFIG", "CONFIG_EXPAND", "CONFIG_FORMAT", "CONFIG_INDEX",
            "CONFIG_VALUE",
        "CONVERT_FORMAT", "CONVERT_TYPE",
        "DEPENDENCY",
        "DEVICE",
        "FILE_ACCESS", "FILE_BACKUP", "FILE_COPY", "FILE_DELETE",
            "FILE_OPEN", "FILE_READ", "FILE_RENAME", "FILE_WRITE",
        "FOLDER", "FOLDER_ACCESS", "FOLDER_COMPRESS", "FOLDER_CREATE",
            "FOLDER_INIT", "FOLDER_REMOVE", "FOLDER_UNCOMPRESS",
        "HTTP_FORMAT", "HTTP_REQUEST",
        "INSTALLATION",
        "JSON_FORMAT",
        "MAKE_DEPEND", "MAKE_MAKE",
        "MODBUS_CRC", "MODBUS_EXCEPTION", "MODBUS_MASTER",
        "NETWORK_ADDRESS", "NETWORK_ADDRESS_RANGE", "NETWORK_PORT",
        "NOT_IMPLEMENTED",
        "OUTPUT_TOO_SHORT",
        "PROCESS_EXIT_CODE", "PROCESS_KILL", "PROCESS_START", "PROCESS_TIMEOUT",
            "PROCESS_WAIT",
        "REG_OPEN", "REG_QUERY", "REG_SET",
        "SOCKET", "SOCKET_ACCEPT", "SOCKET_BIND", "SOCKET_LISTEN",
            "SOCKET_OPTION", "SOCKET_RECEIVE", "SOCKET_SEND", "SOCKET_STARTUP",
        "STATE",
        "TEST",
        "TEXT_FORMAT", "TEXT_TOO_SHORT",
        "TIMEOUT",
        "TODO",
        "THREAD_START",
        "VERSION_FILE", "VERSION_FORMAT", "VERSION_NUMBER",
    };

    unsigned int lCode = static_cast<unsigned int>(aCode);

    assert(static_cast<unsigned int>(KMS::Exception::Code::CODE_QTY) > lCode);

    return CODE_NAMES[lCode];
}
