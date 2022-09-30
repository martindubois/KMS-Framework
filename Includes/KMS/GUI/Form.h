
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/GUI/Form.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/HTTP/ReactApp.h>
#include <KMS/Msg/IReceiver.h>

namespace KMS
{
    namespace GUI
    {

        class Form : public Msg::IReceiver
        {

        public:

            Form();

            ~Form();

            void SetDictionary(DI::Dictionary* aD);

            void Connect(HTTP::ReactApp* aApp, const char* aName);

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        private:

            unsigned int OnGetData    (HTTP::Request* aR);
            unsigned int OnGetMetaData(HTTP::Request* aR);
            unsigned int OnSetData    (HTTP::Request* aR);

            DI::Dictionary* mDictionary;

            DI::Array mFields;

        };

    }
}
