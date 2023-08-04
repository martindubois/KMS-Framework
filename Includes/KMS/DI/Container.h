
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

            // ===== Object =================================================
            virtual ~Container();

        // Internal

            class Entry
            {

            public:

                Entry(const Object* aObject);

                Entry(Object* aObject, bool aDelete);

                operator const Object* () const;

                operator Object* ();

                const Object* Get() const;

                Object* Get();

                bool IsConst() const;

                void Set(const Object* aObject);

                void Set(Object* aObject, bool aDelete);

                void Release();

            private:

                struct
                {
                    unsigned mConst  : 1;
                    unsigned mDelete : 1;
                }
                mFlags;

                Object* mObject;

            };

        protected:

            Container();

            Object* CallCreator();

            bool IsDynamic();

        private:

            Creator mCreator;

        };

    }
}
