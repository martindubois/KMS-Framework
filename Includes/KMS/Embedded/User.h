
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/User.h

#pragma once

#define KMS_EMBEDDED_USER_NAME_LENGTH_MAX (16)

namespace KMS
{
    namespace Embedded
    {

        class IFile;

        class User
        {

        public:

            static const uint32_t ACCESS_DEFAULT;
            static const uint32_t ACCESS_NONE;
            static const uint32_t ACCESS_ROOT;

            User();

            void AddAccess(uint32_t aFeatures);

            void Clear();

            void ClearPassword();

            uint32_t GetAccess() const;

            const char* GetName() const;

            bool Is(const char* aName) const;

            bool IsAllowed(uint32_t aFeatures) const;

            bool IsValid() const;

            void RemoveAccess(uint32_t aFeatures);

            void SetAccess(uint32_t aFeatures);

            void SetName(const char* aName);

            void SetPassword(const char* aPassword, uint32_t aSalt);

        // Internal

            bool Load(IFile* aFile);

            bool Save(IFile* aFile) const;

            bool VerifyPassword(const char* aPassword) const;

        private:

            char mName[KMS_EMBEDDED_USER_NAME_LENGTH_MAX];

            uint32_t mAccess;
            uint32_t mSalt;

            uint64_t mPassword;

        };

    }
}
