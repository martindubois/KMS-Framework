
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Array.h
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

            // ===== Container ==============================================
            virtual unsigned int GetCount() const;
            virtual bool         IsEmpty() const;
            virtual Object     * FindObject_RW(const char* aName);

            // ===== Object =================================================
            virtual ~Array();
            virtual void Clear();

        // Internal

            typedef std::vector<Container::Entry> Internal;

            Internal mInternal;

        };

    }
}
