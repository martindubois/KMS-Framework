
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
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
#include <KMS/Ptr.h>

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

            template <typename T>
            const T* GetEntry_R(int aIndex) const;

            Object* GetEntry_RW(int aIndex);

            template <typename T>
            T* GetEntry_RW(int aIndex);

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

            typedef std::vector<Ptr_OF<DI::Object>> Internal;

            Internal mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        // TODO  Add exception and _Try version

        template <typename T>
        const T* Array::GetEntry_R(int aIndex) const
        {
            return dynamic_cast<const T*>(GetEntry_R(aIndex));
        }

        template <typename T>
        T* Array::GetEntry_RW(int aIndex)
        {
            return dynamic_cast<T*>(GetEntry_RW(aIndex));
        }

    }
}
