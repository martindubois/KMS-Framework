
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Object.h

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

            Array(const MetaData* aMD = NULL);

            void operator += (Object* aIn);

            Object* operator [] (int aIndex);

            const Object* operator [] (int aIndex) const;

            Object* operator [] (const char* aName);

            const Object* operator [] (const char* aName) const;

            void Clear();

            unsigned int GetCount() const;

            bool IsEmpty() const;

            // ===== Object =================================================
            virtual ~Array();

        private:

            typedef std::vector<Object *> ObjectArray;

            ObjectArray mObjects;

        };

        class Dictionary : public Array
        {

        public:

            Dictionary(const MetaData* aMD = NULL);

            // ===== Object =================================================
            virtual ~Dictionary();

        };

    }
}
