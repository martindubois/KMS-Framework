
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Modbus.h

#pragma once

namespace KMS
{
    namespace Modbus
    {

        // Data types
        // //////////////////////////////////////////////////////////////////

        typedef uint16_t Address;

        typedef uint8_t DeviceAddress;

        enum class Exception
        {
            NO_EXCEPTION = 0,

            ILLEGAL_FUNCTION                        =  1,
            ILLEGAL_DATA_ADDRESS                    =  2,
            ILLEGAL_DATA_VALUE                      =  3,
            SLAVE_DEVICE_FAILURE                    =  4,
            ACKNOWLEDGE                             =  5,
            SLAVE_DEVICE_BUSY                       =  6,
            MEMORY_PARITY_ERROR                     =  8,
            GATEWAY_PATH_UNAVAILABLE                = 10,
            GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND = 11,
        };

        enum class Function
        {
            READ_COILS                    =  1, // FC {SAxx QCxx}                         ==> FC BC {Bit...}            (A)
            READ_DISCRETE_INPUTS          =  2, // FC {SAxx QIxx}                         ==> FC BC {Bit...}            (A)
            READ_HOLDING_REGISTERS        =  3, // FC {SAxx QRxx}                         ==> FC BC {UInt16...}         (A)
            READ_INPUT_REGISTERS          =  4, // FC {SAxx QIRx}                         ==> FC BC {UInt16...}         (A)
            WRITE_SINGLE_COIL             =  5, // FC {OAxx 0000|ff00}                    ==> FC {OAxx 0000|ff00}       (B)
            WRITE_SINGLE_REGISTER         =  6, // FC {RAxx RVxx}                         ==> FC {RAxx RVxx}            (B)
            READ_EXCEPTION_STATUS         =  7, // FC                                     ==> FC ...                    (C)
            DIAGNOSTIC                    =  8, // FC ...                                 ==> FC ...                    (D)
            GET_COMM_EVENT_COUNTER        = 11, // FC                                     ==> FC {Sxxx ECxx}            (C)
            GET_COMM_EVENT_LOG            = 12, // FC                                     ==> FC BC ...                 (E)
            WRITE_MULTIPLE_COILS          = 15, // FC SAxx QOxx BC ...                    ==> FC SAxx QOxx              (F)
            WRITE_MULTIPLE_REGISTERS      = 16, // FC SAxx QRxx BC ...                    ==> FC SAxx QRxx              (F)
            REPORT_SERVER_ID              = 17, // FC                                     ==> FC BC ...                 (E)
            MASK_WRITE_REGISTER           = 22, // FC {RAxx Andx Orxx}                    ==> FC {RAxx Andx Orxx}       (D)
            READ_WRITE_MULTIPLE_REGISTERS = 23, // FC {RSAx QRxx WSAxx QWxx BC UInt16...} ==> FC BC {UInt16...}         (A)
            READ_FIFO_QUEUE               = 24, // FC {PAxx}                              ==> FC BCxx {Cxxx UInt16...}  (G)
        };

        typedef uint16_t RegisterValue;

        // Constants
        // //////////////////////////////////////////////////////////////////

        extern const DeviceAddress BROADCAST;

        extern const uint16_t OFF;
        extern const uint16_t ON;

        // Functions
        // //////////////////////////////////////////////////////////////////

        extern bool ReadBit(const uint8_t* aData, unsigned int aOffset, unsigned int aIndex);
        extern void WriteBit(uint8_t* aData, unsigned int aOffset, unsigned int aIndex, bool aValue);

        extern uint8_t ReadUInt8(const uint8_t* aData, unsigned int aOffset);
        extern void    WriteUInt8(uint8_t* aData, unsigned int aOffset, uint8_t aValue);

        extern uint16_t ReadUInt16(const uint8_t* aData, unsigned int aOffset);
        extern void     WriteUInt16(uint8_t* aData, unsigned int aOffset, uint16_t aValue);

    }
}

#ifndef _KMS_EMBEDDED_
    std::ostream& operator << (std::ostream& aOut, KMS::Modbus::Exception aIn);
    std::ostream& operator << (std::ostream& aOut, KMS::Modbus::Function  aIn);
#endif
