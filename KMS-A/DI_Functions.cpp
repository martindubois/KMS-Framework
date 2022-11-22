
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Functions.cpp

// TEST COVERAGE 2022-10-28 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Boolean.h>

#include <KMS/DI/Functions.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void Add   (KMS::DI::Object* aObject, const char* aValue);
static void Assign(KMS::DI::Object* aObject, const char* aValue);
static void Set   (KMS::DI::Object* aObject);

namespace KMS
{
    namespace DI
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        bool Execute_Operation(DI::Container* aRoot, const char* aOperation)
        {
            assert(NULL != aRoot);
            assert(NULL != aOperation);

            char lName    [NAME_LENGTH];
            char lOperator[NAME_LENGTH];
            char lValue   [LINE_LENGTH];

            int lRet = sscanf_s(aOperation, "%[^ +=] %[+=] %[^\n\r\t]", lName SizeInfo(lName), lOperator SizeInfo(lOperator), lValue SizeInfo(lValue));
            KMS_EXCEPTION_ASSERT(1 <= lRet, DI_FORMAT_INVALID, "Invalid operation", aOperation);

            Object* lObject = aRoot->FindObject_RW(lName);

            bool lResult = (NULL != lObject);
            if (lResult)
            {
                switch (lRet)
                {
                case 3:
                    if      (0 == strcmp("+=", lOperator)) { Add   (lObject, lValue); }
                    else if (0 == strcmp("=" , lOperator)) { Assign(lObject, lValue); }
                    break;

                case 2:
                    if (0 == strcmp("=", lOperator))
                    {
                        if (lObject->Clear())
                        {
                            lObject->Send_OnChanged();
                        }
                    }
                    break;

                case 1: Set(lObject); break;

                default: assert(false);
                }
            }

            return lResult;
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void Add(DI::Object* aObject, const char* aValue)
{
    assert(NULL != aValue);

    DI::Array* lArray = dynamic_cast<DI::Array*>(aObject);
    if (NULL != lArray)
    {
        DI::Object* lObject = lArray->CreateEntry();

        Assign(lObject, aValue);

        lArray->Send_OnChanged();

        return;
    }

    KMS_EXCEPTION(DI_FORMAT_INVALID, "Invalid add operation", aValue);
}

void Assign(DI::Object* aObject, const char* aValue)
{
    assert(NULL != aObject);

    DI::Value* lValue = dynamic_cast<DI::Value*>(aObject);
    if (NULL != lValue)
    {
        lValue->Set(aValue);
        return;
    }

    KMS_EXCEPTION(DI_FORMAT_INVALID, "Invalid assign operation", aValue);
}

void Set(DI::Object* aObject)
{
    assert(NULL != aObject);

    DI::Boolean* lBoolean = dynamic_cast<DI::Boolean*>(aObject);
    if (NULL != lBoolean)
    {
        if (!lBoolean->Get())
        {
            lBoolean->Set();

            lBoolean->Send_OnChanged();
        }
        return;
    }

    KMS_EXCEPTION(DI_FORMAT_INVALID, "Invalid set operation", "");
}
