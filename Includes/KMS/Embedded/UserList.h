
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/UserList.h

#pragma once

namespace KMS
{
    namespace Embedded
    {

        class IFile;
        class User;

        class UserList
        {

        public:

            UserList(User* aVector, uint8_t aLength);

            const User* GetCurrentUser() const;

            User* GetUser(uint8_t aIndex);

            User* GetUser(const char* aName);

            uint8_t GetUserCount() const;

            bool IsAllowed(uint32_t aFeatures) const;

            void AutoLogon(IFile* aFile);

            void Login(const char* aName, const char* aPassword);

            void Logout();

            void Load(IFile* aFile);

            bool Save(IFile* aFile) const;

            User* NewUser();

        private:

            User  * mCurrent;
            User  * mVector;
            uint8_t mVectorLength;

        };

    }
}
