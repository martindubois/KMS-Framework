
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Array.h

#pragma once

// ===== C++ ================================================================
#include <vector>

// ===== Includes ===========================================================
#include <KMS/DI/Object.h>

namespace KMS
{
    namespace DI
    {

        class Array : public Object
        {

        public:

            typedef DI::Object* (*Creator)(const KMS::DI::MetaData* aMD);

            static DI::Object* Create(const KMS::DI::MetaData* aMD);

            Array(const MetaData* aMD);

            Object* operator [] (int aIndex);

            const Object* operator [] (int aIndex) const;

            Object* operator [] (const char* aName);

            const Object* operator [] (const char* aName) const;

            void Clear();

            unsigned int GetCount() const;

            bool IsEmpty() const;

            void SetCreator(Creator aCreator);

            void AddEntry(      Object* aE);
            void AddEntry(const Object* aE);

            DI::Object* CreateEntry(const KMS::DI::MetaData* aMD);

            DI::Object* CreateEntry(const char* aName, const char* aLabel, unsigned int aFlags = 0);

            // ===== Object =================================================
            virtual ~Array();

        // Internal

            typedef std::vector<Object*> Internal;

            const Internal& GetInternal() const;

            Internal& GetInternal();

        private:

            Creator mCreator;

            Internal mInternal;

        };

    }
}
