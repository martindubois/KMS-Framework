
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dictionary.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/SafeAPI.h>

#include <KMS/Dictionary.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    bool Dictionary::IsEmpty() const { return (0 >= mEntries.size()); }

    void Dictionary::Set(const char* aName, const char* aValue)
    {
        assert(NULL != aName);
        assert(NULL != aValue);

        Internal::iterator lIt = mEntries.find(aName);
        if (mEntries.end() == lIt)
        {
            mEntries.insert(Internal::value_type(aName, aValue));
        }
        else
        {
            lIt->second = aValue;
        }
    }

    void Dictionary::Set(const char* aName, uint32_t aValue, Radix aRadix)
    {
        char lValue[16];

        switch (aRadix)
        {
        case Radix::DECIMAL    : sprintf_s(lValue, "%u", aValue); break;
        case Radix::HEXADECIMAL: sprintf_s(lValue, "%x", aValue); break;

        default: assert(false);
        }

        Set(aName, lValue);
    }

    unsigned int Dictionary::HTTP_Get(char* aOut, unsigned int aOutSize_byte)
    {
        assert(NULL != aOut);

        unsigned int lResult_byte = 0;

        for (Internal::value_type lPair : mEntries)
        {
            lResult_byte += sprintf_s(aOut + lResult_byte, aOutSize_byte - lResult_byte, "%s: %s\r\n", lPair.first.c_str(), lPair.second.c_str());
        }

        return lResult_byte;
    }

    void Dictionary::HTTP_Set(const char* aHTTP)
    {
        assert(NULL != aHTTP);

        const char* lHTTP = aHTTP;

        for (;;)
        {
            char lName [NAME_LENGTH];
            char lValue[LINE_LENGTH];

            if (2 != sscanf_s(lHTTP, "%[^:]: %[^\r\n]", lName SizeInfo(lName), lValue SizeInfo(lValue)))
            {
                break;
            }

            Set(lName, lValue);

            lHTTP = strchr(lHTTP, '\n');
            if (NULL == lHTTP)
            {
                break;
            }

            lHTTP += 1;
        }
    }

    unsigned int Dictionary::JSON_Get(char* aOut, unsigned int aOutSize_byte)
    {
        assert(NULL != aOut);

        unsigned int lNeeded_byte = 2 + 6 * static_cast<unsigned int>(mEntries.size());

        for (Internal::value_type& lEntry : mEntries)
        {
            lNeeded_byte += static_cast<unsigned int>(lEntry.first.size());
            lNeeded_byte += static_cast<unsigned int>(lEntry.second.size());
        }

        if (aOutSize_byte < lNeeded_byte)
        {
            KMS_EXCEPTION_WITH_INFO(OUTPUT_TOO_SHORT, "The output buffer is too small", lNeeded_byte);
        }

        bool lFirst = true;
        unsigned int lResult_byte = 0;

        aOut[lResult_byte] = '{'; lResult_byte++;

        for (Internal::value_type& lEntry : mEntries)
        {
            if (lFirst)
            {
                lFirst = false;
            }
            else
            {
                aOut[lResult_byte] = ','; lResult_byte++;
            }

            lResult_byte += sprintf_s(aOut + lResult_byte SizeInfoV(aOutSize_byte - lResult_byte), "\"%s\":\"%s\"", lEntry.first.c_str(), lEntry.second.c_str());
        }

        aOut[lResult_byte] = '}'; lResult_byte++;
        aOut[lResult_byte] = '\0';

        assert(lNeeded_byte - 1 == lResult_byte);

        return lResult_byte;
    }

    // Internal
    // //////////////////////////////////////////////////////////////////////

    const Dictionary::Internal& Dictionary::GetInternal() const { return mEntries; }

}

std::ostream& operator << (std::ostream& aOut, const KMS::Dictionary& aD)
{
    const KMS::Dictionary::Internal& lEntries = aD.GetInternal();

    for (KMS::Dictionary::Internal::value_type lPair : lEntries)
    {
        aOut << lPair.first << " : " << lPair.second << std::endl;
    }

    return aOut;
}
