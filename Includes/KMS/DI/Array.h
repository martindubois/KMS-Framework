
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

            void AddEntry(      Object* aE, bool aDelete);
            void AddEntry(const Object* aE);

            DI::Object* CreateEntry();

            const Object* GetEntry_R(int aIndex) const;

            Object* GetEntry_RW(int aIndex);

            // ===== Container ==============================================
            virtual void         Clear();
            virtual unsigned int GetCount() const;
            virtual bool         IsEmpty() const;

            // ===== Object =================================================
            virtual ~Array();

        // Internal

            typedef std::vector<Container::Entry> Internal;

            const Internal& GetInternal() const;

            Internal& GetInternal();

        private:

            Internal mInternal;

        };

    }
}
