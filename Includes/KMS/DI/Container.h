
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

        class Container : public Object
        {

        public:

            void operator += (Object* aIn);

            Object* operator [] (int aIndex);

            const Object* operator [] (int aIndex) const;

            Object* operator [] (const char* aName);

            const Object* operator [] (const char* aName) const;

            void Clear();

            unsigned int GetCount() const;

            // ===== Object =================================================
            virtual ~Container();

        private:

            typedef std::vector<Object *> ObjectArray;

            ObjectArray mObjects;

        };

        class Array : public Container
        {

        public:

            // ===== Object =================================================
            virtual ~Array();

        };

        class Dictionary : public Container
        {

        public:

            // ===== Object =================================================
            virtual ~Dictionary();

        };

    }
}
