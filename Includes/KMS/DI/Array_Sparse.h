
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Array_Sparse.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>

namespace KMS
{
    namespace DI
    {

        class Array_Sparse : public Container
        {

        public:

            static DI::Object* Create();

            void AddConstEntry(int aIndex, const Object* aE);

            void AddEntry(int aIndex, Object* aE, bool aDelete);

            DI::Object* CreateEntry(int aIndex);

            const Object* GetEntry_R(int aIndex) const;

            Object* GetEntry_RW(int aIndex);

            // ===== Container ==============================================
            virtual unsigned int GetCount() const;
            virtual bool         IsEmpty() const;
            virtual Object     * FindObject_RW(const char* aName);

            // ===== Object =================================================
            virtual ~Array_Sparse();
            virtual bool Clear();

        // Internal

            typedef std::map<int, Container::Entry> Internal;

            Internal mInternal;

        };

    }
}
