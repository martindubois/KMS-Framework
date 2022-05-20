
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
            BUILDER_COMPILE = 0,
            BUILDER_TEST,
            CONFIG_FORMAT,
            CONFIG_INDEX,
            CONFIG_TYPE,
            DEPENDENCY,
            FILE_COPY,
            FILE_OPEN,
            FILE_WRITE,
            FOLDER_COMPRESS,
            FOLDER_CREATE,
            FOLDER_INIT,
            FOLDER_REMOVE,
            FOLDER_UNCOMPRESS,
            PROCESS_EXIT_CODE,
            PROCESS_START,
            PROCESS_TIMEOUT,
            TEST,
            VERSION_FILE,
            VERSION_NUMBER,

            CODE_QTY
        };

        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage);
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage, const char * aInfo);
        Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage, uint64_t aInfo);

        Code         GetCode    () const;
        const char * GetCodeName() const;
        const char * GetFile    () const;
        const char * GetFunction() const;
        const char * GetInfo    () const;
        unsigned int GetLine    () const;

    private:

        void Init(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode);

        Code mCode;

        std::string mFile;
        std::string mFunction;

        unsigned int mLine;

        std::string mInfo;

        unsigned int mLastError;

    };

}

std::ostream & operator << (std::ostream & aOut, const KMS::Exception & aE);

std::ostream & operator << (std::ostream & aOut, const KMS::Exception::Code & aC);

#define KMS_CATCH_RESULT(R)                                                \
    catch (KMS::Exception eE)                                              \
    {                                                                      \
        std::cerr << KMS::Console::Color::RED << "EXCEPTION\n";            \
        std::cerr << eE << KMS::Console::Color::WHITE;                     \
        R = __LINE__;                                                      \
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
