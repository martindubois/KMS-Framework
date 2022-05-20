
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Exception.cpp

#include "Component.h"

// ===== C ==================================================================
#include <stdint.h>

// ===== Windows ============================================================
#include <Windows.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static const char* ToCodeName(KMS::Exception::Code aCode);

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage)
        : std::exception(aMessage)
    {
        assert(NULL != aMessage);

        Init(aFile, aFunction, aLine, aCode);
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage, const char* aInfo)
        : std::exception(aMessage)
    {
        assert(NULL != aMessage);

        Init(aFile, aFunction, aLine, aCode);

        mInfo = aInfo;
    }

    Exception::Exception(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode, const char* aMessage, uint64_t aInfo)
        : std::exception(aMessage)
    {
        assert(NULL != aMessage);

        Init(aFile, aFunction, aLine, aCode);

        char lInfo[64];

        sprintf_s(lInfo, "%llu 0x%llx", aInfo, aInfo);

        mInfo = lInfo;
    }

    Exception::Code Exception::GetCode    () const { return mCode; }
    const char    * Exception::GetCodeName() const { return ToCodeName(mCode); }
    const char    * Exception::GetFile    () const { return mFile    .c_str(); }
    const char    * Exception::GetFunction() const { return mFunction.c_str(); }
    const char    * Exception::GetInfo    () const { return mInfo    .c_str(); }
    unsigned int    Exception::GetLine    () const { return mLine; }

    // Provate
    // //////////////////////////////////////////////////////////////////////

    void Exception::Init(const char* aFile, const char* aFunction, unsigned int aLine, Code aCode)
    {
        assert(NULL != aFile);
        assert(NULL != aFunction);
        assert(0 < aLine);
        assert(Code::CODE_QTY > aCode);

        mCode      = aCode;
        mFile      = aFile;
        mFunction  = aFunction;
        mLastError = GetLastError();
        mLine      = aLine;
    }

}

std::ostream& operator << (std::ostream& aOut, const KMS::Exception& aE)
{
    aOut << aE.what() << std::endl;
    aOut << "Code     : " << aE.GetCode    () << std::endl;
    aOut << "File     : " << aE.GetFile    () << std::endl;
    aOut << "Function : " << aE.GetFunction() << std::endl;
    aOut << "Line     : " << aE.GetLine    () << std::endl;
    aOut << "Info     : " << aE.GetInfo    () << std::endl;

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
        "BUILDER_COMPILE", "BUILDER_TEST",
        "CONFIG_FORMAT", "CONFIG_TYPE", "CONFIG_INDEX",
        "DEPENDENCY",
        "FILE_COPY", "FILE_OPEN", "FILE_WRITE",
        "FOLDER_COMPRESS", "FOLDER_CREATE", "FOLDER_INIT", "FOLDER_REMOVE", "FOLDER_UNCOMPRESS"
        "PROCESS_EXIT_CODE", "PROCESS_START", "PROCESS_TIMEOUT",
        "TEST",
        "VERSION_FILE", "VERSION_NUMBER",
    };

    unsigned int lCode = static_cast<unsigned int>(aCode);

    assert(static_cast<unsigned int>(KMS::Exception::Code::CODE_QTY) > lCode);

    return CODE_NAMES[lCode];
}
