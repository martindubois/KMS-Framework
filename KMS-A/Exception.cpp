
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

        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
        Dbg::gLog.WriteException(*this);
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg, const char* aInfo)
    {
        Construct(aFile, aFunction, aLine, aCode, aMsg);

        mInfo = aInfo;

        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
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

        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT);
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

    void Exception::Construct(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg)
    {
        assert(NULL != aFile);
        assert(NULL != aFunction);
        assert(0 < aLine);
        assert(Code::QTY > aCode);
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

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const Exception& aE)
{
    aE.Display(aOut);

    return aOut;
}

std::ostream& operator << (std::ostream& aOut, const Exception::Code& aC)
{
    aOut << ToCodeName(aC);

    return aOut;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

const char* ToCodeName(Exception::Code aCode)
{
    static const char* CODE_NAMES[static_cast<unsigned int>(Exception::Code::QTY)] =
    {
        "APPLICATION_ERROR",
        "APPLICATION_SYSTEM_ERROR",
        "APPLICATION_USER_ERROR",

        "ARG_COUNT_ERROR", "ARG_VALUE_ERROR",

        "BUILD_COMMAND_FAILED"      , "BUILD_COMPILE_FAILED", "BUILD_CONFIG_INVALID",
        "BUILD_DEPENDENCY_NOT_FOUND", "BUILD_TEST_FAILED"   ,

        "CFG_FORMAT_INVALID", "CFG_OPEN_FAILED",

        "CLI_COMMAND_INVALID", "CLI_NAME_INVALID",

        "CLICK_CLASS_INVALID", "CLICK_HANDLE_INVALID"  , "CLICK_KEY_INVALID",
        "CLICK_NAME_INVALID" , "CLICK_OPERATION_FAILED",

        "COM_CLEAR_FAILED"  , "COM_CONFIG_FAILED", "COM_CONFIG_INVALID"  ,
        "COM_CONTROL_FAILED", "COM_DATA_INVALID" , "COM_OUPTUT_TOO_SHORT",

        "CONVERT_DATA_TYPE_INVALID", "CONVERT_FORMAT_INVALID", "CONVERT_OPEN_FAILED",
        "CONVERT_OUTPUT_TOO_SHORT" , "CONVERT_VALUE_INVALID" ,

        "DBG_OPEN_FAILED",

        "DEV_CONFIG_INVALID", "DEV_CONNECT_FAILED", "DEV_READ_FAILED",
        "DEV_WRITE_FAILED"  ,

        "DI_DENIED"          , "DI_FORMAT_INVALID", "DI_INDEX_INVALID",
        "DI_NAME_INVALID"    , "DI_NOT_DYNAMIC"   , "DI_OPEN_FAILED"  ,
        "DI_OUTPUT_TOO_SHORT",

        "ENUM_INVALID_VALUE_NAME",

        "ENV_EXPAND_FAILED", "ENV_OUTPUT_TOO_SHORT",

        "FILE_ACCESS_FAILED" , "FILE_BACKUP_FAILED", "FILE_COMPRESS_FAILED"  
        ,
        "FILE_CONFIG_INVALID", "FILE_COPY_FAILED"  , "FILE_CREATE_FAILED"    ,
        "FILE_DELETE_FAILED" , "FILE_INIT_FAILED"  , "FILE_MAPPING_FAILED"   ,
        "FILE_OPEN_FAILED"   , "FILE_READ_FAILED"  , "FILE_REMOVE_FAILED"    ,
        "FILE_RENAME_FAILED" , "FILE_TOO_SHORT"    , "FILE_UNCOMPRESS_FAILED",

        "FILE_FORMAT_ERROR",

        "HTTP_INSTALLATION_ERROR", "HTTP_NOT_IMPLEMENTED", "HTTP_RECEIVE_ERROR",

        "JSON_FORMAT_INVALID", "JSON_NOT_IMPLEMENTED",

        "MODBUS_CONFIG_INVALID", "MODBUS_CRC_ERROR"       , "MODBUS_ERROR"     ,
        "MODBUS_EXCEPTION"     , "MODBUS_OUTPUT_TOO_SHORT", "MODBUS_RECV_ERROR",

        "NET_ADDRESS_INVALID"      , "NET_ADDRESS_RANGE_INVALID", "NET_ADDRESS_RESOLUTION_FAILED",
        "NET_PORT_INVALID"         , "NET_SOCKET_BIND_FAILED"   , "NET_SOCKET_CONNECT_FAILED"    ,
        "NET_SOCKET_FAILED"        , "NET_SOCKET_LISTEN_FAILED" , "NET_SOCKET_OPTION_FAILED"     ,
        "NET_SOCKET_RECEIVE_FAILED", "NET_SOCKET_SEND_FAILED"   , "NET_SOCKET_STARTUP_FAILED"    ,
        "NET_STATE_INVALID"        ,

        "NOT_IMPLEMENTED",

        "PROC_ACCESS_FAILED", "PROC_KILL_FAILED", "PROC_START_FAILED",
        "PROC_TIMEOUT"      ,

        "REG_OPEN_FAILED", "REG_QUERY_FAILED", "REG_SET_FAILED",

        "TEST",

        "TEXT_ARGUMENT_INVALID", "TEXT_FORMAT_INVALID", "TEXT_OPEN_FAILED",
        "TEXT_OUTPUT_TOO_SHORT", "TEXT_TOO_SHORT"     ,

        "THREAD_START_FAILED", "THREAD_STATE_INVALID",

        "TODO",

        "VERSION_FILE_CORRUPTED", "VERSION_FORMAT_INVALID", "VERSION_NUMBER_INVALID",
    };

    Enum<Exception::Code, CODE_NAMES> lCode(aCode);

    return lCode.GetName();
}
