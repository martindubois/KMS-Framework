
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Array_Sparse.h

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/DI/Object.h>

namespace KMS
{
    namespace DI
    {

        class Array_Sparse : public Object
        {

        public:

            typedef DI::Object* (*Creator)(const KMS::DI::MetaData* aMD);

            static DI::Object* Create(const KMS::DI::MetaData* aMD);

            Array_Sparse(const MetaData* aMD);

            Object* operator [] (int aIndex);

            const Object* operator [] (int aIndex) const;

            Object* operator [] (const char* aName);

            const Object* operator [] (const char* aName) const;

            void Clear();

            unsigned int GetCount() const;

            bool IsEmpty() const;

            void SetCreator(Creator aCreator);

            void AddEntry(int aIndex,       Object* aE);
            void AddEntry(int aIndex, const Object* aE);

            DI::Object* CreateEntry(int aIndex, const KMS::DI::MetaData* aMD);

            DI::Object* CreateEntry(int aIndex, const char* aName, const char* aLabel, unsigned int aFlags = 0);

            // ===== Object =================================================
            virtual ~Array_Sparse();

        private:

            typedef std::map<int, Object *> Internal;

            Creator mCreator;

            Internal mInternal;

        };

    }
}
