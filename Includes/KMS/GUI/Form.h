
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/GUI/Form.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/HTTP/ReactApp.h>

namespace KMS
{
    namespace GUI
    {

        class Form
        {

        public:

            Form();

            ~Form();

            void SetDictionary(DI::Dictionary* aD);

            void Connect(HTTP::ReactApp* aApp, const char* aName);

        private:

            // ===== Callbacks ==============================================
            const Callback<Form> ON_GET_DATA;
            const Callback<Form> ON_GET_META_DATA;
            const Callback<Form> ON_SET_DATA;
            unsigned int OnGetData    (void* aSender, void* aData);
            unsigned int OnGetMetaData(void* aSender, void* aData);
            unsigned int OnSetData    (void* aSender, void* aData);

            DI::Dictionary* mDictionary;

            DI::Array mFields;

        };

    }
}
