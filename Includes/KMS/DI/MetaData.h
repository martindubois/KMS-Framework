
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Object.h

#pragma once

// ===== C++ ================================================================
#include <string>

namespace KMS
{
    namespace DI
    {

        class Object;

        class MetaData
        {

        public:

            static const unsigned int FLAG_DELETE_OBJECT;
            static const unsigned int FLAG_DYNAMIC;

            static const unsigned int FLAG_COPY_NAME;
            static const unsigned int FLAG_COPY_LABEL;
            static const unsigned int FLAG_DELETE_META_DATA;

            MetaData(const char* aName, const char* aLabel, unsigned int aFlags = 0);

            ~MetaData();

            const char* GetName() const;

            unsigned int GetName(char* aOut, unsigned int aOutSize_byte) const;

            const char* GetLabel() const;

        // Internal

            bool Is(const char* aName) const;

            const MetaData* Copy() const;

            bool TestFlag(unsigned int aFlag) const;

        private:

            unsigned int mFlags;

            const char* mLabel;
            const char* mName;

        };

        extern const MetaData META_DATA_DELETE_OBJECT;
        extern const MetaData META_DATA_DELETE_OBJECT_AND_DYNAMIC;
        extern const MetaData META_DATA_DYNAMIC;

    }
}
