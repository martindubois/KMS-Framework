
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Exception.h

#pragma once

// ===== C++ ================================================================
#include <exception>
#include <iostream>

namespace KMS
{

    class Exception : public std::exception
    {

    public:

        enum class Code
        {
            APPLICATION_ERROR = 0,
            APPLICATION_SYSTEM_ERROR,
            APPLICATION_USER_ERROR,

            ARG_COUNT_ERROR, ARG_VALUE_ERROR,

            BUILD_COMMAND_FAILED      , BUILD_COMPILE_FAILED, BUILD_CONFIG_INVALID,
            BUILD_DEPENDENCY_NOT_FOUND, BUILD_TEST_FAILED   ,

            CFG_FORMAT_INVALID, CFG_OPEN_FAILED,

            CLI_COMMAND_INVALID, CLI_NAME_INVALID,

            CLICK_CLASS_INVALID, CLICK_HANDLE_INVALID  , CLICK_KEY_INVALID,
            CLICK_NAME_INVALID , CLICK_OPERATION_FAILED,

            COM_CLEAR_FAILED  , COM_CONFIG_FAILED, COM_CONFIG_INVALID  ,
            COM_CONTROL_FAILED, COM_DATA_INVALID , COM_OUTPUT_TOO_SHORT,

            CONVERT_DATA_TYPE_INVALID, CONVERT_FORMAT_INVALID, CONVERT_OPEN_FAILED,
            CONVERT_OUTPUT_TOO_SHORT , CONVERT_VALUE_INVALID ,

            DBG_OPEN_FAILED,

            DEV_CONFIG_INVALID, DEV_CONNECT_FAILED, DEV_READ_FAILED,
            DEV_WRITE_FAILED  ,

            DI_DENIED          , DI_FORMAT_INVALID  , DI_INDEX_INVALID   ,
            DI_NAME_INVALID    , DI_NOT_DYNAMIC     , DI_OPEN_FAILED     ,
            DI_OUTPUT_TOO_SHORT,

            ENUM_INVALID_VALUE_NAME,

            ENV_EXPAND_FAILED, ENV_OUTPUT_TOO_SHORT,

            FILE_ACCESS_FAILED , FILE_BACKUP_FAILED, FILE_COMPRESS_FAILED  ,
            FILE_CONFIG_INVALID, FILE_COPY_FAILED  , FILE_CREATE_FAILED    ,
            FILE_DELETE_FAILED , FILE_INIT_FAILED  , FILE_MAPPING_FAILED   ,
            FILE_OPEN_FAILED   , FILE_READ_FAILED  , FILE_REMOVE_FAILED    ,
            FILE_RENAME_FAILED , FILE_TOO_SHORT    , FILE_UNCOMPRESS_FAILED,
            FILE_WRITE_FAILED  ,

            FILE_FORMAT_ERROR,

            FOLDER_CHANGE_FAILED,

            HTTP_INSTALLATION_ERROR, HTTP_NOT_IMPLEMENTED, HTTP_RECEIVE_ERROR,

            JSON_FORMAT_INVALID, JSON_NOT_IMPLEMENTED,

            MODBUS_CONFIG_INVALID, MODBUS_CRC_ERROR       , MODBUS_ERROR     ,
            MODBUS_EXCEPTION     , MODBUS_OUTPUT_TOO_SHORT, MODBUS_RECV_ERROR,

            NET_ADDRESS_INVALID      , NET_ADDRESS_RANGE_INVALID, NET_ADDRESS_RESOLUTION_FAILED,
            NET_PORT_INVALID         , NET_SOCKET_BIND_FAILED   , NET_SOCKET_CONNECT_FAILED    ,
            NET_SOCKET_FAILED        , NET_SOCKET_LISTEN_FAILED , NET_SOCKET_OPTION_FAILED     ,
            NET_SOCKET_RECEIVE_FAILED, NET_SOCKET_SEND_FAILED   , NET_SOCKET_STARTUP_FAILED    ,
            NET_STATE_INVALID        ,

            NOT_IMPLEMENTED,

            PROC_ACCESS_FAILED, PROC_KILL_FAILED, PROC_START_FAILED,
            PROC_TIMEOUT      ,

            REG_OPEN_FAILED, REG_QUERY_FAILED, REG_SET_FAILED,

            TEST,

            TEXT_ARGUMENT_INVALID, TEXT_FORMAT_INVALID, TEXT_OPEN_FAILED,
            TEXT_OUTPUT_TOO_SHORT, TEXT_TOO_SHORT     ,

            THREAD_START_FAILED, THREAD_STATE_INVALID,

            TODO,

            VERSION_FILE_CORRUPTED, VERSION_FORMAT_INVALID, VERSION_NUMBER_INVALID,

            WGDI_CONFIG_ERROR, WGDI_CREATE_ERROR,

            QTY
        };

        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg);
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg, const char * aInfo);
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg, uint64_t aInfo);

        Code         GetCode     () const;
        const char * GetCodeName () const;
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

        void Construct(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMsg);

        Code mCode;

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

std::ostream& operator << (std::ostream& aOut, const KMS::Exception::Code& aC);

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
    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::C, (M), (I))

#define KMS_EXCEPTION_ASSERT(A, C, M, I) \
    if (!(A))                            \
    {                                    \
        KMS_EXCEPTION(C, (M), (I));      \
    }
