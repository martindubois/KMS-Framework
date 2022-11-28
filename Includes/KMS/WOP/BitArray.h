
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/BitArray.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/Object.h>
#include <KMS/WOP/WOP.h>

namespace KMS
{
    namespace WOP
    {

        template <typename T>
        class BitArray : public Object
        {

        public:

            BitArray(T aIn = 0);

            void ClearBits(T aMask = 0xff);
            T    GetBits  (T aMask = 0xff) const;
            void SetBits  (T aMask = 0xff);
            bool TestBits (T aMask = 0xff) const;

            void SetData(T aValue);

            // ===== Object =================================================
            virtual bool    PrepareFrame(FrameBuffer* aOut, uint8_t aInstance);
            virtual uint8_t WriteData(const FrameBuffer* aIn);

        private:

            T mBits;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        BitArray<T>::BitArray(T aIn) : mBits(aIn) {}

        template <typename T>
        void BitArray<T>::ClearBits(T aMask) { mBits &= ~ aMask; }

        template <typename T>
        T BitArray<T>::GetBits(T aMask) const { return mBits & aMask; }

        template <typename T>
        void BitArray<T>::SetBits(T aMask) { mBits |= aMask; }

        template <typename T>
        bool BitArray<T>::TestBits(T aMask) const { return 0 != (mBits & aMask); }

        // ===== Object =====================================================

        template <typename T>
        bool BitArray<T>::PrepareFrame(FrameBuffer* aOut, uint8_t aInstance)
        {
            bool lResult = false;

            switch (PopRequest(KMS_WOP_BIT_REQUEST | 0x02))
            {
            case KMS_WOP_BIT_REQUEST: PrepareFrame_REQUEST(aOut, aInstance); lResult = true; break;

            case 0x02:
                aOut->Prepare(aInstance, 1);
                aOut->AddDataBytes(&mBits, sizeof(mBits));
                lResult = true;
                break;
            }

            return lResult;
        }

        template <typename T>
        uint8_t BitArray<T>::WriteData(const FrameBuffer* aIn)
        {
            uint8_t lResult = KMS_WOP_RESULT_INVALID_INSTANCE;

            switch (aIn->GetDataType())
            {
            case KMS_WOP_DATA_TYPE_REQUEST: lResult = WriteData_REQUEST(aIn); break;

            case 1: lResult = aIn->GetData(&mBits, sizeof(mBits)); break;
            }

            return lResult;
        }

    }
}
