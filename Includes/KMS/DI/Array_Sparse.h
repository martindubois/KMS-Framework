
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

            void AddEntry(int aIndex,       Object* aE, bool aDelete);
            void AddEntry(int aIndex, const Object* aE);

            DI::Object* CreateEntry(int aIndex);

            const Object* GetEntry_R(int aIndex) const;

            Object* GetEntry_RW(int aIndex);

            // ===== Container ==============================================
            virtual void         Clear();
            virtual unsigned int GetCount() const;
            virtual bool         IsEmpty() const;

            // ===== Object =================================================
            virtual ~Array_Sparse();

        // Internal

            typedef std::map<int, Container::Entry> Internal;

            Internal mInternal;

        };

    }
}
