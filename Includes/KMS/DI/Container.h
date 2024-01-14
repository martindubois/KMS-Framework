
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Container.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Object.h>

namespace KMS
{
    namespace DI
    {

        class Container : public Object
        {

        public:

            typedef DI::Object* (*Creator)();

            void SetCreator(Creator aCreator);

            virtual unsigned int GetCount() const = 0;

            virtual bool IsEmpty() const = 0;

            // Exception  DI_DENIED
            //            DI_FORMAT_INVALID
            //            DI_NAME_INVALID
            virtual Object* FindObject_RW(const char* aName) = 0;

            virtual void Validate() const = 0;

            // ===== Object =================================================
            virtual ~Container();

        protected:

            Container();

            Object* CallCreator();

            bool IsDynamic();

        private:

            Creator mCreator;

        };

    }
}
