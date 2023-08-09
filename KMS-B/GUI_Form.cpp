
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/GUI_Form.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/DI/UInt.h>
#include <KMS/GUI/MetaData.h>
#include <KMS/HTTP/Request.h>

#include <KMS/GUI/Form.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI::UInt<uint32_t> ZERO(0);

namespace KMS
{
    namespace GUI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Form::Form()
            : mDictionary(nullptr)
            // ===== Callbacks ==============================================
            , ON_GET_DATA     (this, &Form::OnGetData)
            , ON_GET_META_DATA(this, &Form::OnGetMetaData)
            , ON_SET_DATA     (this, &Form::OnSetData)
        {
            mFields.SetCreator(DI::Dictionary::Create);
        }

        Form::~Form() {}

        void Form::SetDictionary(DI::Dictionary* aD)
        {
            mDictionary = aD;

            if (mFields.IsEmpty() && (nullptr != mDictionary))
            {
                unsigned int lCount = 0;

                for (const DI::Dictionary::Internal::value_type lVT : mDictionary->mInternal)
                {
                    auto lMD = dynamic_cast<const GUI::MetaData*>(lVT.second.mMetaData);

                    auto lD = dynamic_cast<DI::Dictionary*>(mFields.CreateEntry());

                    lD->AddEntry("Index", new DI::UInt<uint32_t>(lCount   ), true);
                    lD->AddEntry("Label", new DI::String(lMD->GetLabel()  ), true);
                    lD->AddEntry("Name" , new DI::String(lVT.first.c_str()), true);

                    lD->AddConstEntry("ReadOnly", &ZERO);

                    lCount++;
                }
            }
        }

        void Form::Connect(HTTP::ReactApp* aRA, const char* aName)
        {
            assert(nullptr != aRA);
            assert(nullptr != aName);

            char lPath[128];

            sprintf_s(lPath, "/%s/GetData", aName);
            aRA->AddFunction(lPath, &ON_GET_DATA);

            sprintf_s(lPath, "/%s/GetMetaData", aName);
            aRA->AddFunction(lPath, &ON_GET_META_DATA);

            sprintf_s(lPath, "/%s/SetData", aName);
            aRA->AddFunction(lPath, &ON_SET_DATA);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Callbacks ==================================================

        unsigned int Form::OnGetData(void* aSenser, void* aData)
        {
            assert(nullptr != aData);

            auto lRequest = reinterpret_cast<HTTP::Request*>(aData);

            /* TODO mData.Clear();

            JSON::Array::Internal* lInternal = mMetaData.GetInternal();
            assert(nullptr != lInternal);

            for (JSON::Object* lO : *lInternal)
            {
                JSON::Dictionary* lD = dynamic_cast<JSON::Dictionary*>(lO);
                assert(nullptr != lD);

                const JSON::Object* lON = lD->GetEntry("Name");
                assert(nullptr != lON);

                const JSON::String* lSN = dynamic_cast<const JSON::String*>(lON);
                assert(nullptr != lSN);

                mData += GetData(*lSN);
            }

            mData.IncRefCount();
            aR->mResponseData.SetEntry("Data", &mData); */

            return 0;
        }

        unsigned int Form::OnGetMetaData(void* aSenser, void* aData)
        {
            assert(nullptr != aData);

            auto lRequest = reinterpret_cast<HTTP::Request*>(aData);

            lRequest->mResponseData.AddEntry("Fields", &mFields, false);

            return 0;
        }

        unsigned int Form::OnSetData(void* aSenser, void* aData)
        {
            assert(nullptr != aData);

            auto lRequest = reinterpret_cast<HTTP::Request*>(aData);

            // TODO
            return 0;
        }

    }
}
