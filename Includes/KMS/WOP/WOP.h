
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/WOP.h

#pragma once

// Constants
// //////////////////////////////////////////////////////////////////////////

#define KMS_WOP_CONTROL_VALUE     (0x00)

#define KMS_WOP_DATA_TYPE_REQUEST (0x0)

#define KMS_WOP_BIT_REQUEST (1 << KMS_WOP_DATA_TYPE_REQUEST)

#define KMS_WOP_INSTANCE_SYSTEM   (0xff)

#define KMS_WOP_RESULT_OK                    (0x00)
#define KMS_WOP_RESULT_INVALID_CRC           (0x01)
#define KMS_WOP_RESULT_INVALID_CONTROL_VALUE (0x02)
#define KMS_WOP_RESULT_INVALID_DATA_SIZE     (0x03)
#define KMS_WOP_RESULT_INVALID_DATA_TYPE     (0x04)
#define KMS_WOP_RESULT_INVALID_FRAME_SIZE    (0x05)
#define KMS_WOP_RESULT_INVALID_INSTANCE      (0x06)
#define KMS_WOP_RESULT_INVALID_REQUEST       (0x07)
#define KMS_WOP_RESULT_NO_REQUEST            (0x08)
#define KMS_WOP_RESULT_PARTIAL               (0x09)
#define KMS_WOP_RESULT_TEST                  (0x0a)

// Data type
// //////////////////////////////////////////////////////////////////////////

// mControl        Must be KMS_WOP_CONTROL_VALUE
// mInstance       The index of the instance to write to
// mReserved0      Must be 0
// mDataType       The data type, 0-6 are user defined,
//                 KMS_WOP_DATA_TYPE_REQUEST is used to communicate write
//                 request.
// mDataSize_byte  The size of data into the frame.
typedef union
{
    struct
    {
        unsigned mControl       : 8;
        unsigned mInstance      : 8;
        unsigned mDataType      : 4;
        unsigned mReserved0     : 4;
        unsigned mDataSize_byte : 8;
    }
    mFields;

    uint8_t mBytes[4];
}
KMS_WOP_Header;
