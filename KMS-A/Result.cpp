
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Result.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Result.h>

// Static variable
// //////////////////////////////////////////////////////////////////////////

typedef std::map<uint32_t, const char*> NameMap;

static unsigned int sIndex;
static NameMap    * sNames;

// Static variable
// //////////////////////////////////////////////////////////////////////////

static void AddName(uint32_t aCode, const char* aName);

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    const Result RESULT_OK     ("RESULT_OK"     , Result::Level::LEVEL_SUCCESS, Result::Type::TYPE_GLOBAL, 0);
    const Result RESULT_WARNING("RESULT_WARNING", Result::Level::LEVEL_WARNING, Result::Type::TYPE_GLOBAL, 0);
    const Result RESULT_ERROR  ("RESULT_ERROR"  , Result::Level::LEVEL_ERROR  , Result::Type::TYPE_GLOBAL, 0);

    const Result RESULT_CONNECT_FAILED   ("RESULT_CONNECT_FAILED"   , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffe8);
    const Result RESULT_CRC_ERROR        ("RESULT_CONNECT_FAILED"   , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffe9);
    const Result RESULT_DENIED           ("RESULT_DENIED"           , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffea);
    const Result RESULT_FILE_FORMAT_ERROR("RESULT_FILE_FORMAT_ERROR", Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffeb);
    const Result RESULT_INVALID_CLASS    ("RESULT_INVALID_CLASS"    , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffec);
    const Result RESULT_INVALID_COMMAND  ("RESULT_INVALID_COMMAND"  , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffed);
    const Result RESULT_INVALID_CONFIG   ("RESULT_INVALID_CONFIG"   , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffee);
    const Result RESULT_INVALID_DATA_TYPE("RESULT_INVALID_DATA_TYPE", Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffef);
    const Result RESULT_INVALID_FORMAT   ("RESULT_INVALID_FORMAT"   , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff0);
    const Result RESULT_INVALID_INDEX    ("RESULT_INVALID_INDEX"    , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff1);
    const Result RESULT_INVALID_KEY      ("RESULT_INVALID_KEY"      , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff2);
    const Result RESULT_INVALID_LINE_NO  ("RESULT_INVALID_LINE_NO"  , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff3);
    const Result RESULT_INVALID_NAME     ("RESULT_INVALID_NAME"     , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff4);
    const Result RESULT_INVALID_STATE    ("RESULT_INVALID_STATE"    , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff5);
    const Result RESULT_INVALID_VALUE    ("RESULT_INVALID_VALUE"    , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff6);
    const Result RESULT_NOT_IMPLEMENTED  ("RESULT_NOT_IMPLEMENTED"  , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff7);
    const Result RESULT_OPEN_FAILED      ("RESULT_OPEN_FAILED"      , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff8);
    const Result RESULT_OPERATION_FAILED ("RESULT_OPERATION_FAILED" , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffff9);
    const Result RESULT_OUTPUT_TOO_SHORT ("RESULT_OUTPUT_TOO_SHORT" , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffffa);
    const Result RESULT_READ_FAILED      ("RESULT_READ_FAILED"      , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffffb);
    const Result RESULT_RECEIVE_FAILED   ("RESULT_RECEIVE_FAILED"   , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffffc);
    const Result RESULT_SEND_FAILED      ("RESULT_SEND_FAILED"      , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffffd);
    const Result RESULT_TIMEOUT          ("RESULT_TIMEOUT"          , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xfffffe);
    const Result RESULT_WRITE_FAILED     ("RESULT_WRITE_FAILED"     , Result::Level::LEVEL_ERROR, Result::Type::TYPE_GLOBAL, 0xffffff);

    Result::Result() { mUnion.mCode = 0; }

    Result::Result(uint32_t aValue) { mUnion.mCode = aValue; }

    Result::operator uint32_t () const { return mUnion.mCode; }

    bool Result::operator == (Result aIn) const { return mUnion.mCode == aIn.mUnion.mCode; }

    unsigned int  Result::GetIndex() const { return mUnion.mFields.mIndex; }
    Result::Level Result::GetLevel() const { return static_cast<Level>(mUnion.mFields.mLevel); }

    const char* Result::GetName() const
    {
        assert(nullptr != sNames);

        auto lIt = sNames->find(mUnion.mCode);
        if (sNames->end() == lIt)
        {
            return "Unknown";
        }

        assert(nullptr != lIt->second);

        return lIt->second;
    }

    Result::Type Result::GetType() const { return static_cast<Type>(mUnion.mFields.mType); }

    // Internal
    // //////////////////////////////////////////////////////////////////////

    Result::Result(const char* aName, Level aLevel, Type aType)
    {
        sIndex++;

        mUnion.mCode = 0;

        mUnion.mFields.mIndex = sIndex;
        mUnion.mFields.mLevel = static_cast<unsigned int>(aLevel);
        mUnion.mFields.mType  = static_cast<unsigned int>(aType);

        AddName(mUnion.mCode, aName);
    }

    Result::Result(const char* aName, Level aLevel, Type aType, unsigned int aIndex)
    {
        mUnion.mCode = 0;

        mUnion.mFields.mIndex = aIndex;
        mUnion.mFields.mLevel = static_cast<unsigned int>(aLevel);
        mUnion.mFields.mType  = static_cast<unsigned int>(aType);

        AddName(mUnion.mCode, aName);
    }

    void Result::Display(std::ostream& aOut) const
    {
        aOut << GetName() << " (" << mUnion.mFields.mIndex << ", ";

        switch (static_cast<Level>(mUnion.mFields.mLevel))
        {
        case Level::LEVEL_ERROR  : aOut << "ERROR, "  ; break;
        case Level::LEVEL_SUCCESS: aOut << "SUCCESS, "; break;
        case Level::LEVEL_WARNING: aOut << "WARNING, "; break;

        default: aOut << "Invalid, ";
        }

        switch (static_cast<Type>(mUnion.mFields.mType))
        {
        case Type::TYPE_GLOBAL: aOut << "GLOBAL)"; break;
        case Type::TYPE_MEMBER: aOut << "MEMBER)"; break;
        case Type::TYPE_STATIC: aOut << "STATIC)"; break;

        default: aOut << "Invalid)";
        }
    }

}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, Result aR)
{
    aR.Display(aOut);

    return aOut;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

// Thread  C++ Init
void AddName(uint32_t aCode, const char* aName)
{
    assert(nullptr != aName);

    if (nullptr == sNames)
    {
        sNames = new NameMap;
    }

    sNames->insert(NameMap::value_type(aCode, aName));
}
