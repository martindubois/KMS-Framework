
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Array.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <vector>

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>

namespace KMS
{
    namespace DI
    {

        class Array : public Container
        {

        public:

            static DI::Object* Create();

            void AddConstEntry(const Object* aE);

            void AddEntry(Object* aE, bool aDelete);

            DI::Object* CreateEntry();

            const Object* GetEntry_R(int aIndex) const;

            Object* GetEntry_RW(int aIndex);

            // Exception  DI_INDEX_INVALID
            void RemoveEntry(int aIndex);

            // Exception  DI_INDEX_INVALID
            void SetEntry(int aIndex, Object* aE, bool aDelete);

            // ===== Container ==============================================
            virtual unsigned int GetCount() const;
            virtual bool         IsEmpty() const;
            virtual Object     * FindObject_RW(const char* aName);
            virtual void         Validate() const;

            // ===== Object =================================================
            virtual ~Array();
            virtual bool Clear();

        // Internal

            typedef std::vector<Container::Entry> Internal;

            Internal mInternal;

        };

    }
}
