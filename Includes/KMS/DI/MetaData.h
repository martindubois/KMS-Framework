
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

            static const unsigned int FLAG_COPY_NAME;
            static const unsigned int FLAG_COPY_LABEL;
            static const unsigned int FLAG_DELETE_META_DATA;
            static const unsigned int FLAG_DELETE_OBJECT;

            MetaData(const char* aName, const char* aLabel, unsigned int aFlags = 0);

            ~MetaData();

            const char * GetLabel() const;
            unsigned int GetLabel(char* aOut, unsigned int aOutSize_byte) const;

            const char * GetName() const;
            unsigned int GetName(char* aOut, unsigned int aOutSize_byte) const;

            bool IsLabeled() const;
            bool IsNamed  () const;

        // Internal

            bool Is(const char* aName) const;

            bool TestFlag(unsigned int aFlag) const;

        private:

            unsigned int mFlags;

            const char* mLabel;
            const char* mName;

        };

        extern const MetaData META_DATA_DELETE_OBJECT;

    }
}
