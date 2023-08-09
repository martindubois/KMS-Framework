
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Stats_Value.h
// Status    PROD_READY
// Library   KMS-A

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

            Stats_Value(const char* aName, const char* aUnit = nullptr);

            void Set(T aIn);

            // ===== Stats_Entry ============================================
            virtual void Reset();

        // Internal

            // ===== Stats_Entry ============================================
            virtual void Display(std::ostream& aOut) const;

        protected:

            double       GetAverage() const;
            unsigned int GetCount  () const;
            T            GetLast   () const;
            T            GetMax    () const;
            T            GetMin    () const;
            double       GetStdDev () const;

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

                if (nullptr != mUnit)
                {
                    aOut << " " << mUnit;
                }

                if (1 < mCount)
                {
                    aOut << " ( " << mMin << " ; " << GetAverage() << " ; " << mMax;
                    aOut << " ; n = " << mCount << " ; Std.Dev. = " << GetStdDev() << " )";
                }

                aOut << std::endl;
            }
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        template <class T>
        double Stats_Value<T>::GetAverage() const
        {
            double lDiv = mCount;

            return mSum / lDiv;
        }

        template <class T>
        unsigned int Stats_Value<T>::GetCount() const { return mCount; }

        template <class T>
        T Stats_Value<T>::GetLast() const { return mLast; }

        template <class T>
        T Stats_Value<T>::GetMax() const { return mMax; }

        template <class T>
        T Stats_Value<T>::GetMin() const { return mMin; }

        template <class T>
        double Stats_Value<T>::GetStdDev() const
        {
            double lDivA = mCount;
            double lDivB = mCount - 1;

            return sqrt((mSum2 - ((mSum * mSum) / lDivA)) / lDivB);
        }

    }
}

template <class T>
std::ostream& operator << (std::ostream& aOut, const KMS::Dbg::Stats_Value<T>& aIn);
