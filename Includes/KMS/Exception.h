
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Exception.h

#pragma once

// ===== C++ ================================================================
#include <exception>
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>

namespace KMS
{

    class Exception : public std::exception
    {

    public:

        enum class Code
        {
            BROWSER_START = 0,
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
            HTTP_REQUEST,
            INSTALLATION,
            MAKE_DEPEND, MAKE_MAKE,
            MODBUS_CRC, MODBUS_EXCEPTION, MODBUS_MASTER,
            NETWORK_ADDRESS, NETWORK_ADDRESS_RANGE, NETWORK_PORT,
            OUTPUT_TOO_SHORT,
            PROCESS_EXIT_CODE, PROCESS_KILL, PROCESS_START, PROCESS_TIMEOUT,
                PROCESS_WAIT,
            SOCKET, SOCKET_ACCEPT, SOCKET_BIND, SOCKET_LISTEN,
                SOCKET_OPTION, SOCKET_RECEIVE, SOCKET_SEND, SOCKET_STARTUP,
            STATE,
            TEST,
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

std::ostream& operator << (std::ostream& aOut, const KMS::Exception& aE);

std::ostream& operator << (std::ostream& aOut, const KMS::Exception::Code& aC);

#define KMS_CATCH                                                          \
    catch (KMS::Exception eE)                                              \
    {                                                                      \
        std::cerr << KMS::Console::Color::RED << "EXCEPTION\n";            \
        std::cerr << eE << KMS::Console::Color::WHITE;                     \
    }                                                                      \
    catch (std::exception eE)                                              \
    {                                                                      \
        std::cerr << KMS::Console::Color::RED << "EXCEPTION\n";            \
        std::cerr << eE.what() << std::endl << KMS::Console::Color::WHITE; \
    }                                                                      \
    catch (...)                                                            \
    {                                                                      \
        std::cerr << KMS::Console::Color::RED << "UNKNOWN EXCEPTION\n";    \
        std::cerr << KMS::Console::Color::WHITE;                           \
    }

#define KMS_CATCH_RESULT(R)                                                \
    catch (KMS::Exception eE)                                              \
    {                                                                      \
        std::cerr << KMS::Console::Color::RED << "EXCEPTION\n";            \
        std::cerr << eE << KMS::Console::Color::WHITE;                     \
        R = static_cast<int>(eE.GetCode());                                \
    }                                                                      \
    catch (std::exception eE)                                              \
    {                                                                      \
        std::cerr << KMS::Console::Color::RED << "EXCEPTION\n";            \
        std::cerr << eE.what() << std::endl << KMS::Console::Color::WHITE; \
        R = __LINE__;                                                      \
    }                                                                      \
    catch (...)                                                            \
    {                                                                      \
        std::cerr << KMS::Console::Color::RED << "UNKNOWN EXCEPTION\n";    \
        std::cerr << KMS::Console::Color::WHITE;                           \
        R = __LINE__;                                                      \
    }

#define KMS_EXCEPTION(C, M) \
    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::C, M)

#define KMS_EXCEPTION_WITH_INFO(C, M, I) \
    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::C, M, I)
