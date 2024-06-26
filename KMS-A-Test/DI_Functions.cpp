
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Functions.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Functions.h>

using namespace KMS;

KMS_TEST(DI_Functions_Base, "Auto", sTest_Base)
{
    DI::Array          lA0;
    DI::Boolean        lB0;
    DI::Dictionary     lD0;
    Ptr_OF<DI::Object> lEntry;
    DI::String         lS0;

    lEntry.Set(&lA0, false); lD0.AddEntry("Array"  , lEntry);
    lEntry.Set(&lB0, false); lD0.AddEntry("Boolean", lEntry);
    lEntry.Set(&lS0, false); lD0.AddEntry("String" , lEntry);

    // Execute_Operation
    KMS_TEST_ASSERT(DI::Execute_Operation(&lD0, "Array="));
    KMS_TEST_ASSERT(DI::Execute_Operation(&lD0, "Boolean"));
    KMS_TEST_ASSERT(DI::Execute_Operation(&lD0, "Boolean="));
    KMS_TEST_ASSERT(DI::Execute_Operation(&lD0, "String="));
}

KMS_TEST(DI_Functions_Exception, "Auto", sTest_Exception)
{
    DI::Array          lA0;
    DI::Boolean        lB0;
    DI::Dictionary     lD0;
    Ptr_OF<DI::Object> lEntry;

    lEntry.Set(&lA0, false); lD0.AddEntry("Array"  , lEntry);
    lEntry.Set(&lB0, false); lD0.AddEntry("Boolean", lEntry);

    // Execute_Operation
    try
    {
        DI::Execute_Operation(&lD0, "");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        DI::Execute_Operation(&lD0, "Boolean+=Ignored");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        DI::Execute_Operation(&lD0, "Array=Ignored");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        DI::Execute_Operation(&lD0, "Array");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);
}
