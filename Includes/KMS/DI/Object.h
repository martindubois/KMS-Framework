
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Object.h

#pragma once

namespace KMS
{
    namespace DI
    {

        class MetaData;

        class Object
        {

        public:

            virtual ~Object();

            const MetaData* GetMetaData() const;

            void SetMetaData(const MetaData* aIn);

            const char* GetName() const;

            unsigned int GetName(char* aOut, unsigned int aOutSize_byte) const;

        // Internal

            bool Is(const char* aName) const;

            bool TestFlag(unsigned int aFlag) const;

        protected:

            Object(const MetaData* aMD);

        private:

            void ClearMetaData();

            const MetaData* mMetaData;

        };

    }
}
