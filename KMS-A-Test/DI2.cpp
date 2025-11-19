
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Dbg_Log.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI2/Array.h>
#include <KMS/DI2/BitField.h>
#include <KMS/DI2/Float.h>
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Int.h>
#include <KMS/DI2/UInt.h>
#include <KMS/DI2/Output.h>
#include <KMS/DI2/String.h>
#include <KMS/DI2/Struct.h>

using namespace KMS;

// Data types
// //////////////////////////////////////////////////////////////////////////

typedef struct
{
    uint32_t mReserved0 : 4;
    uint32_t mField0    : 4;

    uint32_t mReserved1 : 24;
}
TestBitField;

typedef struct
{
    uint32_t mField0;
}
TestStruct;

// Constants
// //////////////////////////////////////////////////////////////////////////

static const DI2::BitField_Field TEST_BIT_FIELD_FIELDS[] =
{
    { "mField0", 0x000000f0, 0 },
    
    { nullptr, 0, 0 }
};

static const DI2::Struct_Field TEST_STRUCT_FIELDS[] =
{
    { "mField0", 0, &DI2::TYPE_UINT32 },

    { nullptr, 0, nullptr }
};

static const DI2::Array<uint32_t, 2>                        TYPE_ARRAY(&DI2::TYPE_UINT32);
static const DI2::BitField<uint32_t, TEST_BIT_FIELD_FIELDS> TYPE_TEST_BIT_FIELD(&DI2::TYPE_UINT32);
static const DI2::String<32>                                TYPE_STRING_32;
static const DI2::Struct<TEST_STRUCT_FIELDS>                TYPE_TEST_STRUCT;

// Tests
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(DI2_Array, "auto", sTest_Array)
{
    char lASCII[4096];

    uint32_t lArray[2];
    lArray[0] = 1;
    lArray[1] = 2;
    DI2::Code_ASCII_String(&lArray, &TYPE_ARRAY, sizeof(lASCII), lASCII);
    memset(&lArray, 0, sizeof(lArray));
    DI2::Decode_ASCII_String(&lArray, &TYPE_ARRAY, lASCII);
    KMS_TEST_COMPARE(lArray[0], 1L);
    KMS_TEST_COMPARE(lArray[1], 2L);
}

KMS_TEST(DI2_BitField, "auto", sTest_BitField)
{
    char lASCII[4096];

    TestBitField lTBF;
    memset(&lTBF, 0, sizeof(lTBF));
    lTBF.mField0 = 1;
    DI2::Code_ASCII_String(&lTBF, &TYPE_TEST_BIT_FIELD, sizeof(lASCII), lASCII);
    memset(&lTBF, 0, sizeof(lTBF));
    DI2::Decode_ASCII_String(&lTBF, &TYPE_TEST_BIT_FIELD, lASCII);
    KMS_TEST_ASSERT(1 == lTBF.mField0);
}

KMS_TEST(DI2_Simple, "Auto", sTest_Simple)
{
    char lASCII[4096];

    double lDouble = 1.0;
    DI2::Code_ASCII_String(&lDouble, &DI2::TYPE_DOUBLE, sizeof(lASCII), lASCII);
    lDouble = 0.0;
    DI2::Decode_ASCII_String(&lDouble, &DI2::TYPE_DOUBLE, lASCII);
    KMS_TEST_COMPARE(lDouble, 1.0);

    float lFloat = 1.0;
    DI2::Code_ASCII_String(&lFloat, &DI2::TYPE_FLOAT, sizeof(lASCII), lASCII);
    lFloat = 0.0;
    DI2::Decode_ASCII_String(&lFloat, &DI2::TYPE_FLOAT, lASCII);
    KMS_TEST_COMPARE(lFloat, 1.0);

    int64_t lInt64 = 1;
    DI2::Code_ASCII_String(&lInt64, &DI2::TYPE_INT64, sizeof(lASCII), lASCII);
    lInt64 = 0;
    DI2::Decode_ASCII_String(&lInt64, &DI2::TYPE_INT64, lASCII);
    KMS_TEST_COMPARE(static_cast<int32_t>(lInt64), 1);

    int32_t lInt32 = 1;
    DI2::Code_ASCII_String(&lInt32, &DI2::TYPE_INT32, sizeof(lASCII), lASCII);
    lInt32 = 0;
    DI2::Decode_ASCII_String(&lInt32, &DI2::TYPE_INT32, lASCII);
    KMS_TEST_COMPARE(lInt32, 1);

    int16_t lInt16 = 1;
    DI2::Code_ASCII_String(&lInt16, &DI2::TYPE_INT16, sizeof(lASCII), lASCII);
    lInt16 = 0;
    DI2::Decode_ASCII_String(&lInt16, &DI2::TYPE_INT16, lASCII);
    KMS_TEST_COMPARE(lInt16, 1);

    int8_t lInt8 = 1;
    DI2::Code_ASCII_String(&lInt8, &DI2::TYPE_INT8, sizeof(lASCII), lASCII);
    lInt8 = 0;
    DI2::Decode_ASCII_String(&lInt8, &DI2::TYPE_INT8, lASCII);
    KMS_TEST_COMPARE(lInt8, 1);

    uint64_t lUInt64 = 1;
    DI2::Code_ASCII_String(&lUInt64, &DI2::TYPE_UINT64, sizeof(lASCII), lASCII);
    lUInt64 = 0;
    DI2::Decode_ASCII_String(&lUInt64, &DI2::TYPE_UINT64, lASCII);
    KMS_TEST_COMPARE(static_cast<uint32_t>(lUInt64), 1L);

    uint32_t lUInt32 = 1;
    DI2::Code_ASCII_String(&lUInt32, &DI2::TYPE_UINT32, sizeof(lASCII), lASCII);
    lUInt32 = 0;
    DI2::Decode_ASCII_String(&lUInt32, &DI2::TYPE_UINT32, lASCII);
    KMS_TEST_COMPARE(lUInt32, 1L);

    uint16_t lUInt16 = 1;
    DI2::Code_ASCII_String(&lUInt16, &DI2::TYPE_UINT16, sizeof(lASCII), lASCII);
    lUInt16 = 0;
    DI2::Decode_ASCII_String(&lUInt16, &DI2::TYPE_UINT16, lASCII);
    KMS_TEST_COMPARE(lUInt16, 1L);

    uint8_t lUInt8 = 1;
    DI2::Code_ASCII_String(&lUInt8, &DI2::TYPE_UINT8, sizeof(lASCII), lASCII);
    lUInt8 = 0;
    DI2::Decode_ASCII_String(&lUInt8, &DI2::TYPE_UINT8, lASCII);
    KMS_TEST_COMPARE(lUInt8, 1L);

    char lString[32];
    strcpy_s(lString, "Test");
    DI2::Code_ASCII_String(&lString, &TYPE_STRING_32, sizeof(lASCII), lASCII);
    memset(&lString, 0, sizeof(lString));
    DI2::Decode_ASCII_String(&lString, &TYPE_STRING_32, lASCII);
    KMS_TEST_ASSERT(0 == strcmp("Test", lString));
}

KMS_TEST(DI2_Struct, "Auto", sTest_Struct)
{
    char lASCII[4096];

    TestStruct lTS;
    lTS.mField0 = 1;
    DI2::Code_ASCII_String(&lTS, &TYPE_TEST_STRUCT, sizeof(lASCII), lASCII);
    memset(&lTS, 0, sizeof(lTS));
    DI2::Decode_ASCII_String(&lTS, &TYPE_TEST_STRUCT, lASCII);
    KMS_TEST_COMPARE(lTS.mField0, 1L);
}
