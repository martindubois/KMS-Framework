
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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
            APPLICATION = 0,
            ARGUMENT,
            BROWSER_START,
            BUILD_COMMAND, BUILD_COMPILE, BUILD_TEST,
            COM_CONFIG, COM_CONNECT, COM_CONTROL,
            CONFIG, CONFIG_EXPAND, CONFIG_FORMAT, CONFIG_INDEX,
                CONFIG_VALUE,
            CONVERT_FORMAT, CONVERT_TYPE,
            DEPENDENCY,
            DEVICE,
            FILE_ACCESS, FILE_BACKUP, FILE_COPY, FILE_DELETE,
                FILE_OPEN, FILE_READ, FILE_RENAME, FILE_WRITE,
            FOLDER, FOLDER_ACCESS, FOLDER_COMPRESS, FOLDER_CREATE,
                FOLDER_INIT, FOLDER_REMOVE, FOLDER_UNCOMPRESS,
            HTTP_FORMAT, HTTP_REQUEST,
            INSTALLATION,
            JSON_FORMAT,
            MAKE_DEPEND, MAKE_MAKE,
            MODBUS_CRC, MODBUS_EXCEPTION, MODBUS_MASTER,
            NETWORK_ADDRESS, NETWORK_ADDRESS_RANGE, NETWORK_PORT,
            NOT_IMPLEMENTED,
            OUTPUT_TOO_SHORT,
            PROCESS_EXIT_CODE, PROCESS_KILL, PROCESS_START, PROCESS_TIMEOUT,
                PROCESS_WAIT,
            REG_OPEN, REG_QUERY, REG_SET,
            SOCKET, SOCKET_ACCEPT, SOCKET_BIND, SOCKET_LISTEN,
                SOCKET_OPTION, SOCKET_RECEIVE, SOCKET_SEND, SOCKET_STARTUP,
            STATE,
            TEST,
            TEXT_FORMAT, TEXT_TOO_SHORT,
            TIMEOUT,
            TODO,
            THREAD_START,
            VERSION_FILE, VERSION_FORMAT, VERSION_NUMBER,

            CODE_QTY
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
        KMS_DBG_LOG_ERROR();                                               \
        KMS::Dbg::gLog.WriteException(eE);                                 \
    }                                                                      \
    catch (std::exception eE)                                              \
    {                                                                      \
        KMS_DBG_LOG_ERROR();                                               \
    }                                                                      \
    catch (...)                                                            \
    {                                                                      \
        KMS_DBG_LOG_ERROR();                                               \
    }

#define KMS_CATCH_RESULT(R)                                                \
    catch (KMS::Exception eE)                                              \
    {                                                                      \
        KMS_DBG_LOG_ERROR();                                               \
        KMS::Dbg::gLog.WriteException(eE);                                 \
        R = static_cast<int>(eE.GetCode());                                \
    }                                                                      \
    catch (std::exception eE)                                              \
    {                                                                      \
        KMS_DBG_LOG_ERROR();                                               \
        R = __LINE__;                                                      \
    }                                                                      \
    catch (...)                                                            \
    {                                                                      \
        KMS_DBG_LOG_ERROR();                                               \
        R = __LINE__;                                                      \
    }

#define KMS_EXCEPTION(C, M, I) \
    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::C, (M), (I))

#define KMS_EXCEPTION_ASSERT(A, C, M, I) \
    if (!(A))                            \
    {                                    \
        KMS_EXCEPTION(C, (M), (I));      \
    }
