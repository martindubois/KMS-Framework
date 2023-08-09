
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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

namespace KMS
{
    namespace DI
    {

        class MetaData;

        class Dictionary : public Container
        {

        public:

            static DI::Object* Create();

            void AddConstEntry(const char* aName, const Object* aE, const MetaData* aMD = nullptr);

            void AddEntry(const char* aName, Object* aE, bool aDelete, const MetaData* aMD = nullptr);

            DI::Object* CreateEntry(const char* aName, const MetaData* aMD = nullptr);

            const Object* GetEntry_R(const char* aName) const;

            Object* GetEntry_RW(const char* aName);

            // ===== Container ==============================================
            virtual bool         Clear();
            virtual unsigned int GetCount() const;
            virtual bool         IsEmpty() const;
            virtual Object     * FindObject_RW(const char* aName);

            // ===== Object =================================================
            virtual ~Dictionary();

        // Internal

            class Entry : public Container::Entry
            {

            public:

                Entry(const Object* aObject, const MetaData* aMD);

                Entry(Object* aObject, bool aDelete, const MetaData* aMD);

                const MetaData* mMetaData;

            };

            typedef std::map<std::string, Entry> Internal;

            Internal mInternal;

        };

    }
}
