
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Monitor.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/Master.h>

#include <KMS/Modbus/Monitor.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define COUNT_MAX (8)

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Monitor::Monitor(const Entry* aTable, unsigned int aTable_Len, Master* aMaster)
            : mIndex(0xffffffff)
            , mMaster(aMaster)
            , mTable(aTable)
            , mTable_Len(aTable_Len)
            , mRegValue(0)
        {
            assert(NULL != aTable);
            assert(0 < aTable_Len);
            assert(NULL != aMaster);
        }

        uint16_t Monitor::GetErrorCode() const
        {
            assert(mIndex < mTable_Len);

            return mTable[mIndex].mErrorCode;
        }

        unsigned int Monitor::GetIndex() const
        {
            assert(mIndex < mTable_Len);

            return mIndex;
        }

        RegisterValue Monitor::GetRegisterValue() const { return mRegValue; }

        // TODO  Save modbus operation by using one opertaion to read
        //       consecutive register
        Monitor::Result Monitor::Tick()
        {
            assert(NULL != mTable);
            assert(0 < mTable_Len);

            mIndex++;

            if (mTable_Len <= mIndex) { mIndex = 0; }

            const Entry * lEntry = mTable + mIndex;
            bool          lValueB;

            switch (lEntry->mType)
            {
            case EntryType::BIT:
                assert(lEntry->mMax == lEntry->mMin);

                if (!mMaster->ReadCoil(lEntry->mAddress, &lValueB)) { return Result::MODBUS_ERROR; }

                if (lValueB)
                {
                    if (Modbus::ON != lEntry->mMax) { return Result::DETECTED_ERROR; }
                }
                else
                {
                    if (Modbus::OFF != lEntry->mMax) { return Result::DETECTED_ERROR; }
                }
                break;

            case EntryType::WORD:
                if (!mMaster->ReadInputRegister(lEntry->mAddress, &mRegValue)) { return Result::MODBUS_ERROR; }

                if (lEntry->mMax >= lEntry->mMin)
                {
                    if ((lEntry->mMax < mRegValue) || (lEntry->mMin > mRegValue)) { return Result::DETECTED_ERROR; }
                }
                else
                {
                    if ((lEntry->mMax >= mRegValue) || (lEntry->mMin <= mRegValue)) { return Result::DETECTED_ERROR; }
                } 
                break;

            default: assert(false);
            }

            return Result::NO_ERROR;
        }

    }
}
