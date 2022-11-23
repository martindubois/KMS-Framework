
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_Object_Dynamic.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/WOP.h>

#include <KMS/WOP/Object_Dynamic.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static uint8_t Ln2(uint8_t aValue);

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        unsigned int Object_Dynamic::GetDataSize_byte() const
        {
            unsigned int lResult_byte = 0;

            for (unsigned int i = 0; i < 7; i++)
            {
                lResult_byte += mBuffers[i].GetDataSize_byte();
            }

            return lResult_byte;
        }

        void Object_Dynamic::Dump() const
        {
            for (unsigned int i = 0; i < 7; i++)
            {
                if (0 < mBuffers[i].GetDataSize_byte())
                {
                    std::cout << "Data type " << static_cast<unsigned int>(i + 1) << " :";

                    mBuffers[i].Dump();
                }
            }
        }

        // ===== Object =====================================================

        bool Object_Dynamic::PrepareFrame(FrameBuffer* aOut, uint8_t aInstance)
        {
            bool lResult = false;

            uint8_t lRequest = PopRequest(0xff);
            switch (lRequest)
            {
            case 0: break;

            case KMS_WOP_BIT_REQUEST: PrepareFrame_REQUEST(aOut, aInstance); lResult = true; break;

            default:
                uint8_t lDataType = Ln2(lRequest);
                aOut->Prepare(aInstance, lDataType);
                mBuffers[lDataType - 1].PrepareFrame(aOut);
                lResult = true;
                break;
            }

            return lResult;
        }

        uint8_t Object_Dynamic::WriteData(const FrameBuffer* aIn)
        {
            assert(NULL != aIn);

            uint8_t lResult;

            uint8_t lDataType = aIn->GetDataType();
            switch (lDataType)
            {
            case KMS_WOP_DATA_TYPE_REQUEST: lResult = WriteData_REQUEST(aIn); break;

            default: lResult = mBuffers[lDataType - 1].WriteData(aIn);
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        Object_Dynamic::Buffer::Buffer() : mSize_byte(0) {}

        unsigned int Object_Dynamic::Buffer::GetDataSize_byte() const { return mSize_byte; }

        void Object_Dynamic::Buffer::Dump() const
        {
            std::cout << std::hex;

            for (unsigned int i = 0; i < mSize_byte; i++)
            {
                std::cout << " " << static_cast<unsigned int>(mData[i]);
            }

            std::cout << std::dec << "\n";
        }

        void Object_Dynamic::Buffer::PrepareFrame(FrameBuffer* aOut)
        {
            aOut->AddDataBytes(mData, mSize_byte);
        }

        uint8_t Object_Dynamic::Buffer::WriteData(const FrameBuffer* aIn)
        {
            assert(NULL != aIn);

            mSize_byte = aIn->GetDataSize_byte();

            return aIn->GetData(mData, mSize_byte);
        }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

uint8_t Ln2(uint8_t aValue)
{
    assert(0 != aValue);

    uint8_t lResult = 1;

    while (0 == (aValue & (1 << lResult)))
    {
        lResult++;
    }

    return lResult;
}

