
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Monitor.h
// Status
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Modbus/Modbus.h>

namespace KMS
{
    namespace Modbus
    {

        class Master;

        class Monitor
        {

        public:

            enum class EntryType
            {
                BIT = 0,
                WORD,
            };

            // Type Error if        Min         Max
            // ==== ==============  =========== ===========
            // BIT  false           Modbus::ON  Modbus::ON
            //      true            Modbus::OFF Modbus::OFF
            // WORD outside [A,B]   A           B
            //      inside [A,B]    B           A
            //      <> A            A           A
            //      == A            A + 1       A - 1
            typedef struct
            {

                uint16_t mErrorCode;

                EntryType mType;

                Address mAddress;

                RegisterValue mMax;
                RegisterValue mMin;
            }
            Entry;

            enum class Result
            {
                NO_ERROR = 0,
                DETECTED_ERROR,
                MODBUS_ERROR,
            };

            Monitor(const Entry* aTable, unsigned int aTable_Len, Master* aMaster);

            uint16_t GetErrorCode() const;

            unsigned int GetIndex() const;

            RegisterValue GetRegisterValue() const;

            Result Tick();

        private:

            NO_COPY(Monitor);

            unsigned int mIndex;

            Master* mMaster;

            const Entry* mTable;
            unsigned int mTable_Len;

            RegisterValue mRegValue;

        };

    }
}
