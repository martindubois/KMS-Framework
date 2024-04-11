
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Array.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <map>
#include <string>

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>
#include <KMS/Ptr.h>

namespace KMS
{
    namespace DI
    {

        class MetaData;

        class Dictionary : public Container
        {

        public:

            static DI::Object* Create();

            Dictionary();

            void operator += (const Dictionary& aIn);

            // aE  The value passed here is reset to nullptr when the method
            //     returns.
            void AddEntry(const char* aName, Ptr_OF<Object>& aE, const MetaData* aMD = nullptr);

            DI::Object* CreateEntry(const char* aName, const MetaData* aMD = nullptr);

            const Object* GetEntry_R(const char* aName) const;

            template <typename T>
            const T* GetEntry_R(const char* aName) const;

            Object* GetEntry_RW(const char* aName);

            template <typename T>
            T* GetEntry_RW(const char* aName);

            // ===== Container ==============================================
            virtual bool         Clear();
            virtual unsigned int GetCount() const;
            virtual bool         IsEmpty() const;
            virtual Object     * FindObject_RW(const char* aName);
            virtual void         Validate() const;

            // ===== Object =================================================
            virtual ~Dictionary();

        // Internal

            class Entry : public Ptr_OF<DI::Object>
            {

            public:

                Entry();

                void Set(Ptr_OF<Object>& aObject, const MetaData* aMD);

                const MetaData* mMetaData;

            };

            typedef std::map<std::string, Entry> Internal;

            Internal mInternal;

        private:

            NO_COPY(Dictionary);

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        // TODO  Add Exception and _Try version

        template <typename T>
        const T* Dictionary::GetEntry_R(const char* aName) const
        {
            return dynamic_cast<const T*>(GetEntry_R(aName));
        }

        template <typename T>
        T* Dictionary::GetEntry_RW(const char* aName)
        {
            return dynamic_cast<T*>(GetEntry_RW(aName));
        }

    }
}
