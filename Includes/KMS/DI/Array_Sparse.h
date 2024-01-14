
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Array_Sparse.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>
#include <KMS/Ptr.h>

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

            template <typename T>
            const T* GetEntry_R(int aIndex) const;

            Object* GetEntry_RW(int aIndex);

            template <typename T>
            T* GetEntry_RW(int aIndex);

            // ===== Container ==============================================
            virtual unsigned int GetCount() const;
            virtual bool         IsEmpty() const;
            virtual Object     * FindObject_RW(const char* aName);
            virtual void         Validate() const;

            // ===== Object =================================================
            virtual ~Array_Sparse();
            virtual bool Clear();

        // Internal

            typedef std::map<int, Ptr_OF<DI::Object>> Internal;

            Internal mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        // TODO  Add exception and _Try version

        template <typename T>
        const T* Array_Sparse::GetEntry_R(int aIndex) const
        {
            return dynamic_cast<const T*>(GetEntry_R(aIndex));
        }

        template <typename T>
        T* Array_Sparse::GetEntry_RW(int aIndex)
        {
            return dynamic_cast<T*>(GetEntry_RW(aIndex));
        }

    }
}
