
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/ArgList.cpp

// CODE REVIEW 2026-01-24 Martin Dubois
// TEST COVERAGE 2026-01-26 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/ArgList.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    const unsigned int ArgList::INVALID_USE_COUNT = 0xffff;

    ArgList::ArgList(int aCount, const char** aVector)
        : mCount(aCount), mUseCounts(nullptr), mVector(aVector)
    {
        if (0 < mCount)
        {
            assert(nullptr != mVector);

            mUseCounts = new unsigned int[aCount];

            memset(mUseCounts, 0, sizeof(unsigned int) * mCount);
        }
    }

    ArgList::~ArgList()
    {
        if (nullptr != mUseCounts)
        {
            delete[] mUseCounts;
        }
    }

    const char* ArgList::GetArgument(unsigned int aIndex) const
    {
        const char* lResult = nullptr;

        auto lInternal = GetInternalIndex(aIndex);
        if (mCount > lInternal)
        {
            assert(nullptr != mVector);

            lResult = mVector[lInternal];
            assert(nullptr != lResult);
        }

        return lResult;
    }

    unsigned int ArgList::GetCount() const
    {
        unsigned int lResult = 0;

        for (unsigned int i = 0; i < mCount; i++)
        {
            assert(nullptr != mUseCounts);

            if (INVALID_USE_COUNT != mUseCounts[i])
            {
                lResult++;
            }
        }

        return lResult;
    }

    unsigned int ArgList::GetUseCount(unsigned int aIndex) const
    {
        unsigned int lResult = INVALID_USE_COUNT;

        auto lInternal = GetInternalIndex(aIndex);
        if (mCount > lInternal)
        {
            assert(nullptr != mUseCounts);

            lResult = mUseCounts[lInternal];
        }

        return lResult;
    }

    void ArgList::Display(std::ostream& aOut)
    {
        for (unsigned int i = 0; i < mCount; i++)
        {
            assert(nullptr != mUseCounts);
            assert(nullptr != mVector);

            if (INVALID_USE_COUNT != mUseCounts[i])
            {
                aOut << mVector[i] << "\n";
            }
        }
    }

    void ArgList::IncUseCount(unsigned int aIndex)
    {
        auto lInternal = GetInternalIndex(aIndex);
        if (mCount > lInternal)
        {
            assert(nullptr != mUseCounts);

            if (INVALID_USE_COUNT != mUseCounts[lInternal])
            {
                mUseCounts[lInternal]++;

                assert(INVALID_USE_COUNT != mUseCounts[lInternal]);
            }
        }
    }

    void ArgList::RemoveUsed()
    {
        for (unsigned int i = 0; i < mCount; i++)
        {
            assert(nullptr != mUseCounts);

            if ((INVALID_USE_COUNT != mUseCounts[i]) && (0 < mUseCounts[i]))
            {
                mUseCounts[i] = INVALID_USE_COUNT;
            }
        }
    }

    // Private
    // //////////////////////////////////////////////////////////////////////

    unsigned int ArgList::GetInternalIndex(unsigned int aIndex) const
    {
        unsigned int lIndex = 0;

        for (unsigned int i = 0; i < mCount; i++)
        {
            assert(nullptr != mUseCounts);

            if (INVALID_USE_COUNT != mUseCounts[i])
            {
                if (aIndex == lIndex)
                {
                    return i;
                }

                lIndex++;
            }
        }

        return mCount;
    }

}
