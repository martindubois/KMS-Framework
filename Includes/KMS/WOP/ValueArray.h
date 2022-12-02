
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/ValueArray.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/Object.h>
#include <KMS/WOP/WOP.h>

namespace KMS
{
    namespace WOP
    {

        template <typename T, unsigned int N>
        class ValueArray : public Object
        {

        public:

            ValueArray();

            void Clear();

            T    GetValue(unsigned int aIndex) const;
            void SetValue(unsigned int aIndex, T aIn);

            // ===== Object =================================================
            virtual bool    PrepareFrame(FrameBuffer* aOut, uint8_t aInstance);
            virtual uint8_t WriteData(const FrameBuffer* aIn);

        private:

            T mValues[N];

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T, unsigned int N>
        ValueArray<T, N>::ValueArray() { memset(&mValues, 0, sizeof(mValues)); }

        template <typename T, unsigned int N>
        void ValueArray<T, N>::Clear()
        {
            for (unsigned int i = 0; i < N; i++)
            {
                SetValue(i, 0);
            }
        }

        template <typename T, unsigned int N>
        T ValueArray<T, N>::GetValue(unsigned int aIndex) const { return mValues[aIndex]; }

        template <typename T, unsigned int N>
        void ValueArray<T, N>::SetValue(unsigned int aIndex, T aIn)
        {
            if (mValues[aIndex] != aIn)
            {
                mValues[aIndex] = aIn;
                AddRequest(0x02);
            }
        }

        // ===== Object =====================================================

        template <typename T, unsigned int N>
        bool ValueArray<T, N>::PrepareFrame(FrameBuffer* aOut, uint8_t aInstance)
        {
            bool lResult = false;

            switch (PopRequest(KMS_WOP_BIT_REQUEST | 0x02))
            {
            case KMS_WOP_BIT_REQUEST: PrepareFrame_REQUEST(aOut, aInstance); lResult = true; break;

            case 0x02:
                aOut->Prepare(aInstance, 1);
                aOut->AddDataBytes(&mValues, sizeof(mValues));
                lResult = true;
                break;
            }

            return lResult;
        }

        template <typename T, unsigned int N>
        uint8_t ValueArray<T, N>::WriteData(const FrameBuffer* aIn)
        {
            uint8_t lResult = KMS_WOP_RESULT_INVALID_INSTANCE;

            switch (aIn->GetDataType())
            {
            case KMS_WOP_DATA_TYPE_REQUEST: lResult = WriteData_REQUEST(aIn); break;

            case 1: lResult = aIn->GetData(&mValues, sizeof(mValues)); break;
            }

            return lResult;
        }

    }
}
