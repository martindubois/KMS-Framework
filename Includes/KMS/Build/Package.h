
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Package.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Version.h>

namespace KMS
{
    namespace Build
    {

        class Package final
        {

        public:

            Package(bool mOSIndependent = false);

            void GetFileName(char* aOut, unsigned int aOutSize_byte) const;

            const char* GetProductName() const;

            void GetTagName(char* aOut, unsigned int aOutSize_byte) const;

            const Version& GetVersion() const;

            void Set(const char* aProduct, const Version& aVersion);

            void Parse(const char* aIn);

        private:

            bool        mOSIndependent;
            std::string mProduct;
            Version     mVersion;

        };

    }
}
