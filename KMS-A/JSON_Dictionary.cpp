
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON_Dictionary.cpp

// TEST COVERAGE KMS - Martin Dubois, P. Eng. 2022-08-23

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/JSON.h>
#include <KMS/JSON/Value.h>

#include <KMS/JSON/Dictionary.h>

namespace KMS
{
    namespace JSON
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Dictionary::Dictionary() {}

        Dictionary::Dictionary(const Dictionary& aD) { *this += aD; }

        const Dictionary& Dictionary::operator = (const Dictionary& aD)
        {
            Clear();

            *this += aD;

            return *this;
        }

        void Dictionary::operator += (const Dictionary& aD)
        {
            const Internal& lD = aD.GetInternal();

            for (const Internal::value_type& lPair : lD)
            {
                assert(NULL != lPair.second);

                SetEntry(lPair.first.c_str(), lPair.second->Copy());
            }
        }

        const Base* Dictionary::GetEntry(const char* aKey) const
        {
            assert(NULL != aKey);

            Internal::const_iterator lIt = mEntries.find(aKey);
            if (mEntries.end() == lIt)
            {
                return NULL;
            }

            assert(NULL != lIt->second);

            return lIt->second;
        }

        void Dictionary::SetEntry(const char* aKey, Base* aE)
        {
            assert(NULL != aKey);
            assert(NULL != aE);

            Internal::iterator lIt = mEntries.find(aKey);
            if (mEntries.end() == lIt)
            {
                mEntries.insert(Internal::value_type(aKey, aE));
            }
            else
            {
                assert(NULL != lIt->second);

                delete lIt->second;

                lIt->second = aE;
            }
        }

        // ===== Base =======================================================

        Dictionary::~Dictionary() { Clear(); }

        void Dictionary::Clear()
        {
            for (Internal::value_type& lPair : mEntries)
            {
                assert(NULL != lPair.second);

                delete lPair.second;
            }

            mEntries.clear();
        }

        Base* Dictionary::Copy() const { return new Dictionary(*this); }

        bool Dictionary::IsEmpty() const { return mEntries.empty(); }

        unsigned int Dictionary::HTTP_Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            for (const Internal::value_type& lPair : mEntries)
            {
                assert(NULL != lPair.second);

                if (aOutSize_byte < lResult_byte + 3 + lPair.first.size())
                {
                    KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output buffer is too small");
                }

                lResult_byte += sprintf_s(aOut + lResult_byte, aOutSize_byte - lResult_byte, "%s: ", lPair.first.c_str());
                lResult_byte += lPair.second->HTTP_Get(aOut + lResult_byte, aOutSize_byte - lResult_byte);

                if (aOutSize_byte < lResult_byte + 3)
                {
                    KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output buffer is too small");
                }

                sprintf_s(aOut + lResult_byte, aOutSize_byte - lResult_byte, "\r\n"); lResult_byte += 2;
            }

            return lResult_byte;
        }

        unsigned int Dictionary::HTTP_Set(const char* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);

            unsigned int lResult_byte = 0;
            while (aInSize_byte > lResult_byte)
            {
                switch (aIn[lResult_byte])
                {
                case ' ':
                case '\n':
                case '\r':
                case '\t': lResult_byte++; break;

                default:
                    char lName[NAME_LENGTH];

                    if (1 != sscanf_s(aIn + lResult_byte, "%[^:]:", lName SizeInfo(lName)))
                    {
                        return lResult_byte;
                    }

                    lResult_byte += static_cast<unsigned int>(strlen(lName) + 1);

                    Base* lBase;

                    lResult_byte += CreateFromHTTP(aIn + lResult_byte, aInSize_byte - lResult_byte, &lBase);

                    SetEntry(lName, lBase);
                }
            }

            return lResult_byte;
        }

        unsigned int Dictionary::JSON_Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            bool lFirst = true;

            unsigned int lResult_byte = 0;
            if (aOutSize_byte < 3)
            {
                KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output buffer is too small");
            }

            aOut[lResult_byte] = '{'; lResult_byte++;

            for (const Internal::value_type& lEntry : mEntries)
            {
                assert(NULL != lEntry.second);

                if (aOutSize_byte < lResult_byte + 5 + lEntry.first.size())
                {
                    KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output buffer is too small");
                }

                if (lFirst)
                {
                    lFirst = false;
                }
                else
                {
                    aOut[lResult_byte] = ','; lResult_byte++;
                }

                lResult_byte += sprintf_s(aOut + lResult_byte SizeInfoV(aOutSize_byte - lResult_byte), "\"%s\":", lEntry.first.c_str());
                lResult_byte += lEntry.second->JSON_Get(aOut + lResult_byte SizeInfoV(aOutSize_byte - lResult_byte));
            }

            if (aOutSize_byte < lResult_byte + 2)
            {
                // NOT TESTED
                KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output buffer is too small");
            }

            aOut[lResult_byte] = '}'; lResult_byte++;
            aOut[lResult_byte] = '\0';

            return lResult_byte;
        }

        unsigned int Dictionary::JSON_Set(const char* aIn, unsigned int aInSize_byte)
        {
            KMS_EXCEPTION(TODO, "Not implement yet");
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const Dictionary::Internal& Dictionary::GetInternal() const { return mEntries; }

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Dictionary& aD)
{
    const KMS::JSON::Dictionary::Internal& lEntries = aD.GetInternal();

    for (KMS::JSON::Dictionary::Internal::value_type lPair : lEntries)
    {
        aOut << lPair.first << " : " << *lPair.second << std::endl;
    }

    return aOut;
}
