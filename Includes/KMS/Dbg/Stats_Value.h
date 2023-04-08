
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Stats_Value.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/Dbg/Stats_Entry.h>

namespace KMS
{
    namespace Dbg
    {

        template <class T>
        class Stats_Value : public Stats_Entry
        {

        public:

            Stats_Value(const char* aName, const char* aUnit = NULL);

            void Set(T aIn);

            // ===== Stats_Entry ============================================
            virtual const char* GetType() const;
            virtual void        Reset();

        // Internal

            // ===== Stats_Entry ============================================
            virtual void Display(std::ostream& aOut) const;

        private:

            unsigned int mCount;

            T mLast;
            T mMax;
            T mMin;

            double mSum;
            double mSum2;

            const char* mUnit;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <class T>
        Stats_Value<T>::Stats_Value(const char* aName, const char* aUnit) : Stats_Entry(aName), mUnit(aUnit)
        {
            Reset();
        }

        template <class T>
        void Stats_Value<T>::Set(T aIn)
        {
            mCount++;
            mLast = aIn;

            if (1 == mCount)
            {
                mMax = mLast;
                mMin = mLast;
            }
            else
            {
                if (mMax < mLast) { mMax = mLast; }
                if (mMin > mLast) { mMin = mLast; }
            }

            mSum  += mLast;
            mSum2 += mLast * mLast;
        }

        // ===== Stats_Entry ================================================

        template <class T>
        const char* Stats_Value<T>::GetType() const { return "Stats_Value"; }

        template <class T>
        void Stats_Value<T>::Reset() { mCount = 0; mSum = 0.0; }

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Stats_Entry ================================================

        template <class T>
        void Stats_Value<T>::Display(std::ostream& aOut) const
        {
            if (0 < mCount)
            {
                aOut << GetName() << " : " << mLast;

                if (NULL != mUnit)
                {
                    aOut << " " << mUnit;
                }

                double lAvg = mSum / static_cast<double>(mCount);

                aOut << " ( " << mMin << " ; " << lAvg << " ; " << mMax << " ; n = " << mCount;

                if (1 < mCount)
                {
                    double lSD = (mSum2 - (mSum * mSum)) / static_cast<double>(mCount - 1);

                    aOut << " ; S.D. = " << lSD;
                }

                aOut << " )" << std::endl;
            }
        }

    }
}

template <class T>
std::ostream& operator << (std::ostream& aOut, const KMS::Dbg::Stats_Value<T>& aIn);
