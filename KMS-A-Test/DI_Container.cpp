
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Dbg_Log.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI::MetaData MD_DYNAMIC(NULL, NULL, KMS::DI::MetaData::FLAG_DELETE_OBJECT);
static const KMS::DI::MetaData MD_ROOT   ("Root", "Root");

KMS_TEST(DI_Container_Base, "DI_Container_Base", "Auto", sTest_Base)
{
    char lBuffer[1024];

    KMS::DI::Container lC0;
    KMS::DI::Container lC1;
    KMS::DI::Container lC2;

    KMS::DI::String *lS0;

    unsigned int lFlags;

    // Object::GetMetaData
    KMS_TEST_ASSERT(NULL == lC0.GetMetaData());

    // Object::SetMetaData

    lC0.SetMetaData(&MD_ROOT);

    KMS_TEST_ASSERT(&MD_ROOT == lC0.GetMetaData());

    lFlags = KMS::DI::MetaData::FLAG_DELETE_META_DATA;
    lC1.SetMetaData(new KMS::DI::MetaData("Root", "Root", lFlags));

    lFlags = KMS::DI::MetaData::FLAG_COPY_LABEL | KMS::DI::MetaData::FLAG_COPY_NAME | KMS::DI::MetaData::FLAG_DELETE_META_DATA;
    lC2.SetMetaData(new KMS::DI::MetaData("Root", "Root", lFlags));

    // Container:: +=

    lS0 = new KMS::DI::String();
    lS0->SetMetaData(&MD_DYNAMIC);

    lC0 += lS0;

    // String::Get
    KMS_TEST_COMPARE(lS0->Get(lBuffer, sizeof(lBuffer)), 0UL);

    // String::Set
    lS0->Set("Test");
}

KMS_TEST(DI_Container_Exception, "DI_Container_Exception", "Auto", sTest_Exception)
{
    char lBuffer[1024];

    KMS::DI::String lS0;

    // String::Get
    for (unsigned int lSize_byte = 0; lSize_byte < 2; lSize_byte++)
    {
        try
        {
            KMS_TEST_EXPECTED_EXCEPTION();
            lS0.Get(lBuffer, lSize_byte);
            KMS_TEST_ASSERT(false);
        }
        KMS_TEST_CATCH(OUTPUT_TOO_SHORT);
    }
}
