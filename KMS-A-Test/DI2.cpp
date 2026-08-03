
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Dbg_Log.cpp

#include "Component.h"

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/DI2/Array.h>
#include <KMS/DI2/BitField.h>
#include <KMS/DI2/Float.h>
#include <KMS/DI2/GUID.h>
#include <KMS/DI2/Input.h>
#include <KMS/DI2/Int.h>
#include <KMS/DI2/Output.h>
#include <KMS/DI2/String.h>
#include <KMS/DI2/Struct.h>
#include <KMS/DI2/UInt.h>

using namespace KMS;

// Data types
// //////////////////////////////////////////////////////////////////////////

typedef struct
{
    const DI2::IType* mType;

    const char* mInput;
    const char* mOutput;

    const char* mException;
}
TestCase;

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
    { "mField0", 0x000000f0, 4 },
    
    { nullptr, 0, 0 }
};

static const DI2::Struct_Field TEST_STRUCT_FIELDS[] =
{
    { "mField0", offsetof(TestStruct, mField0), &DI2::TYPE_UINT32 },

    { nullptr, 0, nullptr }
};

static const DI2::Array<uint32_t, 2>                        TYPE_ARRAY(&DI2::TYPE_UINT32);
static const DI2::BitField<uint32_t, TEST_BIT_FIELD_FIELDS> TYPE_TEST_BIT_FIELD(&DI2::TYPE_UINT32);
static const DI2::String<32>                                TYPE_STRING_32;
static const DI2::Struct<TEST_STRUCT_FIELDS>                TYPE_TEST_STRUCT;

static const TestCase TEST_CASES[] =
{
    // ===== DI2/Array.h ====================================================

    // [0] op Value0
    { &TYPE_ARRAY,     "[0]=1", "{1,0}" },
    { &TYPE_ARRAY,    "[0]= 2", "{2,0}" }, // 1 space
    { &TYPE_ARRAY,    "[0] =3", "{3,0}" },
    { &TYPE_ARRAY,    "[0 ]=4", "{4,0}" },
    { &TYPE_ARRAY,    "[ 0]=5", "{5,0}" },
    { &TYPE_ARRAY,   "[0] = 6", "{6,0}" }, // 2 spaces
    { &TYPE_ARRAY,   "[0 ] =7", "{7,0}" },
    { &TYPE_ARRAY,   "[ 0 ]=8", "{8,0}" },
    { &TYPE_ARRAY,  "[0 ] = 9", "{9,0}" }, // 3 spaces
    { &TYPE_ARRAY,  "[ 0 ] =9", "{9,0}" },
    { &TYPE_ARRAY, "[ 0 ] = 9", "{9,0}" }, // 4 spaces

    // { Value0, Value1 }
    { &TYPE_ARRAY,     "{1}", "{1,0}" },
    { &TYPE_ARRAY,    "{2 }", "{2,0}" }, // 1 space
    { &TYPE_ARRAY,    "{ 3}", "{3,0}" },
    { &TYPE_ARRAY,    " {4}", "{4,0}" },
    { &TYPE_ARRAY,   "{ 5 }", "{5,0}" }, // 2 spaces
    { &TYPE_ARRAY,   " { 6}", "{6,0}" },
    { &TYPE_ARRAY,  " { 7 }", "{7,0}" }, // 3 spaces

    // = { Value0; Value1; }
    { &TYPE_ARRAY,       "={1,2}", "{1,2}" },
    { &TYPE_ARRAY,      "={2,3} ", "{2,3}" }, // 1 space
    { &TYPE_ARRAY,      "={3,4 }", "{3,4}" },
    { &TYPE_ARRAY,      "={4, 5}", "{4,5}" },
    { &TYPE_ARRAY,      "={5 ,6}", "{5,6}" },
    { &TYPE_ARRAY,      "={ 6,7}", "{6,7}" },
    { &TYPE_ARRAY,      "= {7,8}", "{7,8}" },
    { &TYPE_ARRAY,      " ={8,9}", "{8,9}" },
    { &TYPE_ARRAY,     "={1, 2 }", "{1,2}" }, // 2 spaces
    { &TYPE_ARRAY,     "={2 , 3}", "{2,3}" },
    { &TYPE_ARRAY,     "={ 3 ,4}", "{3,4}" },
    { &TYPE_ARRAY,     "= { 4,5}", "{4,5}" },
    { &TYPE_ARRAY,     " = {5,6}", "{5,6}" },
    { &TYPE_ARRAY,    "={6, 7 } ", "{6,7}" }, // 3 spaces
    { &TYPE_ARRAY,    "={7 , 8 }", "{7,8}" },
    { &TYPE_ARRAY,    "={ 8 , 9}", "{8,9}" },
    { &TYPE_ARRAY,    "= { 1 ,2}", "{1,2}" },
    { &TYPE_ARRAY,    " = { 2,3}", "{2,3}" },
    { &TYPE_ARRAY,   "={3 , 4 } ", "{3,4}" }, // 4 spaces
    { &TYPE_ARRAY,   "={ 4 , 5 }", "{4,5}" },
    { &TYPE_ARRAY,   "= { 5 , 6}", "{5,6}" },
    { &TYPE_ARRAY,   " = { 6 ,7}", "{6,7}" },
    { &TYPE_ARRAY,  "={ 7 , 8 } ", "{7,8}" }, // 5 spaces
    { &TYPE_ARRAY,  "= { 8 , 9 }", "{8,9}" },
    { &TYPE_ARRAY,  " = { 1 , 2}", "{1,2}" },
    { &TYPE_ARRAY, " = { 2 , 3 }", "{2,3}" }, // 6 spaces

    // Exceptions
    { &TYPE_ARRAY,    "[2]=1", nullptr, "RESULT_INVALID_INDEX" },
    { &TYPE_ARRAY, "={0,1,2}", nullptr, "RESULT_INVALID_INDEX" },

    // ===== DI2/BitField.h =================================================

    // Value
    { &TYPE_TEST_BIT_FIELD,     "100",        "100" },
    { &TYPE_TEST_BIT_FIELD,    " 101",        "101" },

    // op Value

    { &TYPE_TEST_BIT_FIELD,    "=102",        "102" },
    { &TYPE_TEST_BIT_FIELD,   "= 103",        "103" },
    { &TYPE_TEST_BIT_FIELD,  " = 104",        "104" },

    { &TYPE_TEST_BIT_FIELD,   "+=105",        "105" },
    { &TYPE_TEST_BIT_FIELD,  "+= 106",        "106" },
    { &TYPE_TEST_BIT_FIELD, " += 107",        "107" },

    { &TYPE_TEST_BIT_FIELD,   "-=108", "4294967188" },
    { &TYPE_TEST_BIT_FIELD,  "-= 109", "4294967187" },
    { &TYPE_TEST_BIT_FIELD, " -= 110", "4294967186" },

    { &TYPE_TEST_BIT_FIELD,   "*=111",          "0" },
    { &TYPE_TEST_BIT_FIELD,  "*= 112",          "0" },
    { &TYPE_TEST_BIT_FIELD, " *= 113",          "0" },

    { &TYPE_TEST_BIT_FIELD,   "/=114",          "0" },
    { &TYPE_TEST_BIT_FIELD,  "/= 115",          "0" },
    { &TYPE_TEST_BIT_FIELD, " /= 116",          "0" },

    { &TYPE_TEST_BIT_FIELD,   "|=117",        "117" },
    { &TYPE_TEST_BIT_FIELD,  "|= 118",        "118" },
    { &TYPE_TEST_BIT_FIELD, " |= 119",        "119" },

    { &TYPE_TEST_BIT_FIELD,   "&=120",          "0" },
    { &TYPE_TEST_BIT_FIELD,  "&= 121",          "0" },
    { &TYPE_TEST_BIT_FIELD, " &= 122",          "0" },

    { &TYPE_TEST_BIT_FIELD,   "^=123",        "123" },
    { &TYPE_TEST_BIT_FIELD,  "^= 124",        "124" },
    { &TYPE_TEST_BIT_FIELD, " ^= 125",        "125" },

    // Field0 op Value0

    { &TYPE_TEST_BIT_FIELD,     "mField0=1",  "16" },
    { &TYPE_TEST_BIT_FIELD,    "mField0= 1",  "16" },
    { &TYPE_TEST_BIT_FIELD,    "mField0 =1",  "16" },
    { &TYPE_TEST_BIT_FIELD,    " mField0=1",  "16" },
    { &TYPE_TEST_BIT_FIELD,   "mField0 = 1",  "16" },
    { &TYPE_TEST_BIT_FIELD,   " mField0 =1",  "16" },
    { &TYPE_TEST_BIT_FIELD,  " mField0 = 1",  "16" },

    { &TYPE_TEST_BIT_FIELD,    "mField0+=2",  "32" },
    { &TYPE_TEST_BIT_FIELD,   "mField0+= 2",  "32" },
    { &TYPE_TEST_BIT_FIELD,   "mField0 +=2",  "32" },
    { &TYPE_TEST_BIT_FIELD,   " mField0+=2",  "32" },
    { &TYPE_TEST_BIT_FIELD,  "mField0 += 2",  "32" },
    { &TYPE_TEST_BIT_FIELD,  " mField0 +=2",  "32" },
    { &TYPE_TEST_BIT_FIELD, " mField0 += 2",  "32" },

    { &TYPE_TEST_BIT_FIELD,    "mField0-=3", "208" },
    { &TYPE_TEST_BIT_FIELD,   "mField0-= 3", "208" },
    { &TYPE_TEST_BIT_FIELD,   "mField0 -=3", "208" },
    { &TYPE_TEST_BIT_FIELD,   " mField0-=3", "208" },
    { &TYPE_TEST_BIT_FIELD,  "mField0 -= 3", "208" },
    { &TYPE_TEST_BIT_FIELD,  " mField0 -=3", "208" },
    { &TYPE_TEST_BIT_FIELD, " mField0 -= 3", "208" },

    { &TYPE_TEST_BIT_FIELD,    "mField0*=4",   "0" },
    { &TYPE_TEST_BIT_FIELD,   "mField0*= 4",   "0" },
    { &TYPE_TEST_BIT_FIELD,   "mField0 *=4",   "0" },
    { &TYPE_TEST_BIT_FIELD,   " mField0*=4",   "0" },
    { &TYPE_TEST_BIT_FIELD,  "mField0 *= 4",   "0" },
    { &TYPE_TEST_BIT_FIELD,  " mField0 *=4",   "0" },
    { &TYPE_TEST_BIT_FIELD, " mField0 *= 4",   "0" },

    { &TYPE_TEST_BIT_FIELD,    "mField0/=5",   "0" },
    { &TYPE_TEST_BIT_FIELD,   "mField0/= 5",   "0" },
    { &TYPE_TEST_BIT_FIELD,   "mField0 /=5",   "0" },
    { &TYPE_TEST_BIT_FIELD,   " mField0/=5",   "0" },
    { &TYPE_TEST_BIT_FIELD,  "mField0 /= 5",   "0" },
    { &TYPE_TEST_BIT_FIELD,  " mField0 /=5",   "0" },
    { &TYPE_TEST_BIT_FIELD, " mField0 /= 5",   "0" },

    { &TYPE_TEST_BIT_FIELD,    "mField0|=6",  "96" },
    { &TYPE_TEST_BIT_FIELD,   "mField0|= 6",  "96" },
    { &TYPE_TEST_BIT_FIELD,   "mField0 |=6",  "96" },
    { &TYPE_TEST_BIT_FIELD,   " mField0|=6",  "96" },
    { &TYPE_TEST_BIT_FIELD,  "mField0 |= 6",  "96" },
    { &TYPE_TEST_BIT_FIELD,  " mField0 |=6",  "96" },
    { &TYPE_TEST_BIT_FIELD, " mField0 |= 6",  "96" },

    { &TYPE_TEST_BIT_FIELD,    "mField0&=6",   "0" },
    { &TYPE_TEST_BIT_FIELD,   "mField0&= 6",   "0" },
    { &TYPE_TEST_BIT_FIELD,   "mField0 &=6",   "0" },
    { &TYPE_TEST_BIT_FIELD,   " mField0&=6",   "0" },
    { &TYPE_TEST_BIT_FIELD,  "mField0 &= 6",   "0" },
    { &TYPE_TEST_BIT_FIELD,  " mField0 &=6",   "0" },
    { &TYPE_TEST_BIT_FIELD, " mField0 &= 6",   "0" },

    { &TYPE_TEST_BIT_FIELD,    "mField0^=7", "112" },
    { &TYPE_TEST_BIT_FIELD,   "mField0^= 7", "112" },
    { &TYPE_TEST_BIT_FIELD,   "mField0 ^=7", "112" },
    { &TYPE_TEST_BIT_FIELD,   " mField0^=7", "112" },
    { &TYPE_TEST_BIT_FIELD,  "mField0 ^= 7", "112" },
    { &TYPE_TEST_BIT_FIELD,  " mField0 ^=7", "112" },
    { &TYPE_TEST_BIT_FIELD, " mField0 ^= 7", "112" },

    // .Field0 op Value0

    { &TYPE_TEST_BIT_FIELD,    ".mField0=1",  "16" },
    { &TYPE_TEST_BIT_FIELD,   ".mField0= 1",  "16" },
    { &TYPE_TEST_BIT_FIELD,   ".mField0 =1",  "16" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0 = 1",  "16" },

    { &TYPE_TEST_BIT_FIELD,   ".mField0+=2",  "32" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0+= 2",  "32" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0 +=2",  "32" },
    { &TYPE_TEST_BIT_FIELD, ".mField0 += 2",  "32" },

    { &TYPE_TEST_BIT_FIELD,   ".mField0-=3", "208" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0-= 3", "208" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0 -=3", "208" },
    { &TYPE_TEST_BIT_FIELD, ".mField0 -= 3", "208" },

    { &TYPE_TEST_BIT_FIELD,   ".mField0*=4",   "0" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0*= 4",   "0" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0 *=4",   "0" },
    { &TYPE_TEST_BIT_FIELD, ".mField0 *= 4",   "0" },

    { &TYPE_TEST_BIT_FIELD,   ".mField0/=5",   "0" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0/= 5",   "0" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0 /=5",   "0" },
    { &TYPE_TEST_BIT_FIELD, ".mField0 /= 5",   "0" },

    { &TYPE_TEST_BIT_FIELD,   ".mField0|=6",  "96" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0|= 6",  "96" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0 |=6",  "96" },
    { &TYPE_TEST_BIT_FIELD, ".mField0 |= 6",  "96" },

    { &TYPE_TEST_BIT_FIELD,   ".mField0&=6",   "0" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0&= 6",   "0" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0 &=6",   "0" },
    { &TYPE_TEST_BIT_FIELD, ".mField0 &= 6",   "0" },

    { &TYPE_TEST_BIT_FIELD,   ".mField0^=7", "112" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0^= 7", "112" },
    { &TYPE_TEST_BIT_FIELD,  ".mField0 ^=7", "112" },
    { &TYPE_TEST_BIT_FIELD, ".mField0 ^= 7", "112" },

    // { Field0 op Value0; Field1 op Value1; }
    { &TYPE_TEST_BIT_FIELD,       "{mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      "{mField0=0; }", "0" }, // 1 space
    { &TYPE_TEST_BIT_FIELD,      "{mField0=0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      "{mField0= 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      "{mField0 =0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      "{ mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      " {mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,     "{mField0=0 ; }", "0" }, // 2 spaces
    { &TYPE_TEST_BIT_FIELD,     "{mField0= 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,     "{mField0 = 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,     "{ mField0 =0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,     " { mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,    "{mField0= 0 ; }", "0" }, // 3 spaces
    { &TYPE_TEST_BIT_FIELD,    "{mField0 = 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,    "{ mField0 = 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,    " { mField0 =0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,   "{mField0 = 0 ; }", "0" }, // 4 spaces
    { &TYPE_TEST_BIT_FIELD,   "{ mField0 = 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,   " { mField0 = 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,  "{ mField0 = 0 ; }", "0" }, // 5 spaces
    { &TYPE_TEST_BIT_FIELD,  " { mField0 = 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD, " { mField0 = 0 ; }", "0" }, // 6 spaces

    // = { Field0 op Value0; Field1 op Value1; }
    { &TYPE_TEST_BIT_FIELD,        "={mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,       "={mField0=0; }", "0" }, // 1 space
    { &TYPE_TEST_BIT_FIELD,       "={mField0=0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,       "={mField0= 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,       "={mField0 =0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,       "={ mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,       "= {mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,       " ={mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      "={mField0=0 ; }", "0" }, // 2 spaces
    { &TYPE_TEST_BIT_FIELD,      "={mField0= 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      "={mField0 = 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      "={ mField0 =0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      "= { mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,      " = {mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,     "={mField0= 0 ; }", "0" }, // 3 spaces
    { &TYPE_TEST_BIT_FIELD,     "={mField0 = 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,     "={ mField0 = 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,     "= { mField0 =0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,     " = { mField0=0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,    "={mField0 = 0 ; }", "0" }, // 4 spaces
    { &TYPE_TEST_BIT_FIELD,    "={ mField0 = 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,    "= { mField0 = 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,    " = { mField0 =0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,   "={ mField0 = 0 ; }", "0" }, // 5 spaces
    { &TYPE_TEST_BIT_FIELD,   "= { mField0 = 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD,   " = { mField0 = 0;}", "0" },
    { &TYPE_TEST_BIT_FIELD,  "= { mField0 = 0 ; }", "0" }, // 6 spaces
    { &TYPE_TEST_BIT_FIELD,  " = { mField0 = 0 ;}", "0" },
    { &TYPE_TEST_BIT_FIELD, " = { mField0 = 0 ; }", "0" }, // 7 spaces

    // Exception
    { &TYPE_TEST_BIT_FIELD, " = { Invalid; }"    , nullptr, "RESULT_INVALID_FORMAT" },
    { &TYPE_TEST_BIT_FIELD, " = { Invalid = 0; }", nullptr, "RESULT_INVALID_NAME"   },

        // ===== DI2/Float.h ====================================================

        // Value
    { &DI2::TYPE_FLOAT,  "0.1", "0.100000" },
    { &DI2::TYPE_FLOAT, "0.2 ", "0.200000" }, // 1 space
    { &DI2::TYPE_FLOAT, " 0.3", "0.300000" },

    // op Value

    { &DI2::TYPE_FLOAT,    "=0.1", "0.100000" },
    { &DI2::TYPE_FLOAT,   "=0.2 ", "0.200000" }, // 1 space
    { &DI2::TYPE_FLOAT,   "= 0.3", "0.300000" },
    { &DI2::TYPE_FLOAT,   " =0.4", "0.400000" },
    { &DI2::TYPE_FLOAT,  "= 0.5 ", "0.500000" }, // 2 space
    { &DI2::TYPE_FLOAT,  " = 0.6", "0.600000" },
    { &DI2::TYPE_FLOAT, " = 0.7 ", "0.700000" }, // 3 space

    { &DI2::TYPE_FLOAT,    "+=0.1", "0.100000" },
    { &DI2::TYPE_FLOAT,   "+=0.2 ", "0.200000" }, // 1 space
    { &DI2::TYPE_FLOAT,   "+= 0.3", "0.300000" },
    { &DI2::TYPE_FLOAT,   " +=0.4", "0.400000" },
    { &DI2::TYPE_FLOAT,  "+= 0.5 ", "0.500000" }, // 2 space
    { &DI2::TYPE_FLOAT,  " += 0.6", "0.600000" },
    { &DI2::TYPE_FLOAT, " += 0.7 ", "0.700000" }, // 3 space

    { &DI2::TYPE_FLOAT,    "-=0.1", "-0.100000" },
    { &DI2::TYPE_FLOAT,   "-=0.2 ", "-0.200000" }, // 1 space
    { &DI2::TYPE_FLOAT,   "-= 0.3", "-0.300000" },
    { &DI2::TYPE_FLOAT,   " -=0.4", "-0.400000" },
    { &DI2::TYPE_FLOAT,  "-= 0.5 ", "-0.500000" }, // 2 space
    { &DI2::TYPE_FLOAT,  " -= 0.6", "-0.600000" },
    { &DI2::TYPE_FLOAT, " -= 0.7 ", "-0.700000" }, // 3 space

    { &DI2::TYPE_FLOAT,    "*=0.1", "0.000000" },
    { &DI2::TYPE_FLOAT,   "*=0.2 ", "0.000000" }, // 1 space
    { &DI2::TYPE_FLOAT,   "*= 0.3", "0.000000" },
    { &DI2::TYPE_FLOAT,   " *=0.4", "0.000000" },
    { &DI2::TYPE_FLOAT,  "*= 0.5 ", "0.000000" }, // 2 space
    { &DI2::TYPE_FLOAT,  " *= 0.6", "0.000000" },
    { &DI2::TYPE_FLOAT, " *= 0.7 ", "0.000000" }, // 3 space

    { &DI2::TYPE_FLOAT,    "/=0.1", "0.000000" },
    { &DI2::TYPE_FLOAT,   "/=0.2 ", "0.000000" }, // 1 space
    { &DI2::TYPE_FLOAT,   "/= 0.3", "0.000000" },
    { &DI2::TYPE_FLOAT,   " /=0.4", "0.000000" },
    { &DI2::TYPE_FLOAT,  "/= 0.5 ", "0.000000" }, // 2 space
    { &DI2::TYPE_FLOAT,  " /= 0.6", "0.000000" },
    { &DI2::TYPE_FLOAT, " /= 0.7 ", "0.000000" }, // 3 space

    // Exceptions
    { &DI2::TYPE_FLOAT, " /= 0", nullptr, "RESULT_INVALID_VALUE" },

    // ===== DI2/Int.h ======================================================

    // Value
    { &DI2::TYPE_INT32,  "-1", "-1" },
    { &DI2::TYPE_INT32, "-2 ", "-2" },
    { &DI2::TYPE_INT32, " -3", "-3" },

    // op Value

    { &DI2::TYPE_INT32,    "=-4", "-4" },
    { &DI2::TYPE_INT32,   "=-5 ", "-5" }, // 1 space
    { &DI2::TYPE_INT32,   "= -6", "-6" },
    { &DI2::TYPE_INT32,   " =-7", "-7" },
    { &DI2::TYPE_INT32,  "= -8 ", "-8" }, // 2 space
    { &DI2::TYPE_INT32,  " = -9", "-9" },
    { &DI2::TYPE_INT32, " = -1 ", "-1" }, // 3 space

    { &DI2::TYPE_INT32,    "+=-2", "-2" },
    { &DI2::TYPE_INT32,   "+=-3 ", "-3" }, // 1 space
    { &DI2::TYPE_INT32,   "+= -4", "-4" },
    { &DI2::TYPE_INT32,   " +=-5", "-5" },
    { &DI2::TYPE_INT32,  "+= -6 ", "-6" }, // 2 space
    { &DI2::TYPE_INT32,  " += -7", "-7" },
    { &DI2::TYPE_INT32, " += -8 ", "-8" }, // 3 space

    { &DI2::TYPE_INT32,    "-=-9", "9" },
    { &DI2::TYPE_INT32,   "-=-1 ", "1" }, // 1 space
    { &DI2::TYPE_INT32,   "-= -2", "2" },
    { &DI2::TYPE_INT32,   " -=-3", "3" },
    { &DI2::TYPE_INT32,  "-= -4 ", "4" }, // 2 space
    { &DI2::TYPE_INT32,  " -= -5", "5" },
    { &DI2::TYPE_INT32, " -= -6 ", "6" }, // 3 space

    { &DI2::TYPE_INT32,    "*=-7", "0" },
    { &DI2::TYPE_INT32,   "*=-8 ", "0" }, // 1 space
    { &DI2::TYPE_INT32,   "*= -9", "0" },
    { &DI2::TYPE_INT32,   " *=-1", "0" },
    { &DI2::TYPE_INT32,  "*= -2 ", "0" }, // 2 space
    { &DI2::TYPE_INT32,  " *= -3", "0" },
    { &DI2::TYPE_INT32, " *= -4 ", "0" }, // 3 space

    { &DI2::TYPE_INT32,    "/=-5", "0" },
    { &DI2::TYPE_INT32,   "/=-6 ", "0" }, // 1 space
    { &DI2::TYPE_INT32,   "/= -7", "0" },
    { &DI2::TYPE_INT32,   " /=-8", "0" },
    { &DI2::TYPE_INT32,  "/= -9 ", "0" }, // 2 space
    { &DI2::TYPE_INT32,  " /= -1", "0" },
    { &DI2::TYPE_INT32, " /= -2 ", "0" }, // 3 space

    { &DI2::TYPE_INT32, " |= 1", "1" },

    { &DI2::TYPE_INT32, " &= 2", "0" },

    { &DI2::TYPE_INT32, " ^= 3", "3" },

    // Exception
    { &DI2::TYPE_INT32, "3000000000", nullptr, "RESULT_INVALID_VALUE" },
    { &DI2::TYPE_INT32,       "/= 0", nullptr, "RESULT_INVALID_VALUE" },

    // ===== DI2/String.h ===================================================

    // Value
    { &TYPE_STRING_32, "Alpha", "\"Alpha\"" },

    // "Value"
    { &TYPE_STRING_32, "\"Bravo\"", "\"Bravo\"" },

    // op Value
    { &TYPE_STRING_32, "=Charlie", "\"Charlie\"" },
    { &TYPE_STRING_32, "+=Delta" , "\"Delta\""   },

    // op "Value"
    { &TYPE_STRING_32, "=\"Echo\""    , "\"Echo\""    },
    { &TYPE_STRING_32, "+=\"Foxtrot\"", "\"Foxtrot\"" },

    // Exceptions
    { &TYPE_STRING_32, "=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" , nullptr, "RESULT_INVALID_FORMAT" },
    { &TYPE_STRING_32, "+=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", nullptr, "RESULT_INVALID_FORMAT" },
    { &TYPE_STRING_32, "-=String"                              , nullptr, "RESULT_INVALID_FORMAT" },
    { &TYPE_STRING_32, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"  , nullptr, "RESULT_INVALID_FORMAT" },

    // ===== DI2/Struct.h ===================================================

    // Field0 op Value0
    { &TYPE_TEST_STRUCT, "mField0 = 1", "{mField0=1;}" },

    // .Field0 op Value0
    { &TYPE_TEST_STRUCT, ".mField0 = 2", "{mField0=2;}" },

    // { Field0 op Value0; Field1 op Value0; }
    { &TYPE_TEST_STRUCT, "{ mField0 = 3; }", "{mField0=3;}" },

    // = { Field0 op Value0; Field1 op Value0; }
    { &TYPE_TEST_STRUCT, "= { mField0 = 4; }", "{mField0=4;}" },

    // Exceptions
    { &TYPE_TEST_STRUCT, "= { Invalid = 5; }", nullptr, "RESULT_INVALID_NAME" },

    // ===== DI2/UInt.h =====================================================

    // Value
    { &DI2::TYPE_UINT32,   "1", "1" },
    { &DI2::TYPE_UINT32, "0x2", "2" },

    // op Value

    { &DI2::TYPE_UINT32,   "= 3", "3" },
    { &DI2::TYPE_UINT32, "= 0x4", "4" },

    { &DI2::TYPE_UINT32,   " += 5", "5" },
    { &DI2::TYPE_UINT32, " += 0x6", "6" },

    // Exceptions
    { &DI2::TYPE_UINT32, "5000000000", nullptr, "RESULT_INVALID_VALUE" },
    { &DI2::TYPE_UINT32,      " /= 0", nullptr, "RESULT_INVALID_VALUE" },

    // ===== DI2_GUID_W.cpp =================================================

    // {00000000-0000-0000-0000-000000000000}
    { &DI2::TYPE_GUID, "{00000001-0002-0003-0004-000000000005}", "\"{00000001-0002-0003-0004-000000000005}\"" },

    // "{00000000-0000-0000-0000-000000000000}"
    { &DI2::TYPE_GUID, "\"{00000006-0007-0008-0009-00000000000A}\"", "\"{00000006-0007-0008-0009-00000000000A}\"" },

    // op {00000000-0000-0000-0000-000000000000}
    { &DI2::TYPE_GUID,   "={00000001-0002-0003-0004-000000000005}", "\"{00000001-0002-0003-0004-000000000005}\"" },
    { &DI2::TYPE_GUID,  "= {00000001-0002-0003-0004-000000000005}", "\"{00000001-0002-0003-0004-000000000005}\"" },
    { &DI2::TYPE_GUID,  " ={00000001-0002-0003-0004-000000000005}", "\"{00000001-0002-0003-0004-000000000005}\"" },
    { &DI2::TYPE_GUID, " = {00000001-0002-0003-0004-000000000005}", "\"{00000001-0002-0003-0004-000000000005}\"" },

    // op "{00000000-0000-0000-0000-000000000000}"
    { &DI2::TYPE_GUID,   "=\"{00000006-0007-0008-0009-00000000000A}\"", "\"{00000006-0007-0008-0009-00000000000A}\"" },
    { &DI2::TYPE_GUID,  "= \"{00000006-0007-0008-0009-00000000000A}\"", "\"{00000006-0007-0008-0009-00000000000A}\"" },
    { &DI2::TYPE_GUID,  " =\"{00000006-0007-0008-0009-00000000000A}\"", "\"{00000006-0007-0008-0009-00000000000A}\"" },
    { &DI2::TYPE_GUID, " = \"{00000006-0007-0008-0009-00000000000A}\"", "\"{00000006-0007-0008-0009-00000000000A}\"" },

    { nullptr, nullptr, nullptr }
};

// Tests
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(DI2_ArgList, "Auto", sTest_ArgList)
{
    static const char* ARGUMENTS[] = { "mField0=0", "DoesNotExist=1" };

    ArgList lAL0(0, nullptr);
    ArgList lAL1(1, ARGUMENTS);
    ArgList lAL2(2, ARGUMENTS);
    TestStruct lTS0;
    TestStruct lTS1;
    TestStruct lTS2;

    DI2::Decode_ASCII_Arguments(&lTS0, &TYPE_TEST_STRUCT, &lAL0);

    DI2::Decode_ASCII_Arguments(&lTS1, &TYPE_TEST_STRUCT, &lAL1);
    KMS_TEST_COMPARE(lAL1.GetUseCount(0), 1U);
    lAL1.Display(std::cout);
    lAL1.RemoveUsed();
    lAL1.Display(std::cout);

    DI2::Decode_ASCII_Arguments(&lTS2, &TYPE_TEST_STRUCT, &lAL2);
    KMS_TEST_COMPARE(lAL2.GetUseCount(1), 0U);
    KMS_TEST_COMPARE(lAL2.GetUseCount(2), ArgList::INVALID_USE_COUNT);
}

KMS_TEST(DI2_Array, "Auto", sTest_Array)
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

KMS_TEST(DI2_BitField, "Auto", sTest_BitField)
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

KMS_TEST(DI2_Cases, "Auto", sTest_Cases)
{
    unsigned int lCase = 0;

    while (nullptr != TEST_CASES[lCase].mType)
    {
        uint8_t lData[4096];
        char    lOutput[LINE_LENGTH];
        auto    lTest = TEST_CASES + lCase;

        std::cout << "Test case " << lCase << " : " << lTest->mInput << std::endl;

        memset(&lData, 0, sizeof(lData));

        if (nullptr != lTest->mOutput)
        {
            DI2::Decode_ASCII_String(lData, lTest->mType, lTest->mInput);
            DI2::Code_ASCII_String  (lData, lTest->mType, sizeof(lOutput), lOutput);

            KMS_TEST_ASSERT(0 == strcmp(lOutput, lTest->mOutput));
        }
        else
        {
            try
            {
                DI2::Decode_ASCII_String(lData, lTest->mType, lTest->mInput);
                DI2::Code_ASCII_String  (lData, lTest->mType, sizeof(lOutput), lOutput);

                KMS_TEST_ASSERT(false);
            }
            catch (Exception eE)
            {
                Result lR(eE.GetCode());

                KMS_TEST_ASSERT(0 == strcmp(lTest->mException, lR.GetName()));
            }
        }

        lCase++;
    }
}

KMS_TEST(DI2_GUID, "Auto", sTest_GUID)
{
    #ifdef _KMS_WINDOWS_

        char lASCII[4096];

        // {F374BABB-38FA-415D-A8AF-0C3CF05246F6}
        ::GUID lG0 = { 0xf374babb, 0x38fa, 0x415d, { 0xa8, 0xaf, 0xc, 0x3c, 0xf0, 0x52, 0x46, 0xf6 } };
        ::GUID lG1;

        DI2::Code_ASCII_String(&lG0, &DI2::TYPE_GUID, sizeof(lASCII), lASCII);
        memset(&lG1, 0, sizeof(lG1));
        DI2::Decode_ASCII_String(&lG1, &DI2::TYPE_GUID, lASCII);
        KMS_TEST_ASSERT(0 == memcmp(&lG0, &lG1, sizeof(lG0)));

    #endif
}

KMS_TEST(DI2_Input, "Auto", sTest_Input)
{
    TestStruct lTS3;

    KMS_TEST_ASSERT( DI2::Decode_ASCII_String_Try(&lTS3, &TYPE_TEST_STRUCT, "mField0=0"));
    KMS_TEST_ASSERT(!DI2::Decode_ASCII_String_Try(&lTS3, &TYPE_TEST_STRUCT, "mDield1=0"));
}

KMS_TEST(DI2_Input_Exception, "Auto", sTest_Input_Exception)
{
    DI2::Input lI0;
    DI2::Input lI1;
    DI2::Input lI2;

    // Init_File
    try
    {
        lI0.Init_File("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_OPEN_FAILED);

    // Decode_ASCII_String_Try
    TestStruct lTS3;

    try
    {
        DI2::Decode_ASCII_String_Try(&lTS3, &TYPE_TEST_STRUCT, "mField0=+0");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);
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
