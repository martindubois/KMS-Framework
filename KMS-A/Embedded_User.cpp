
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Embedded_User.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/IFile.h>

#include <KMS/Embedded/User.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static uint64_t Hash(const char* aPassword, uint32_t aSalt);

namespace KMS
{
    namespace Embedded
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        User::User() { Clear(); }

        void User::AddAccess(uint32_t aFeatures) { mAccess |= aFeatures; }

        void User::Clear()
        {
            mAccess = ACCESS_NONE;

            mName[0] = '\0';

            ClearPassword();
        }

        void User::ClearPassword()
        {
            mPassword = 0;
            mSalt     = 0;
        }

        uint32_t User::GetAccess() const { return mAccess; }

        const char* User::GetName() const { return mName; }

        bool User::Is(const char* aName) const
        {
            return 0 == strcmp(mName, aName);
        }

        bool User::IsAllowed(uint32_t aFeatures) const
        {
            return aFeatures == (mAccess & aFeatures);
        }

        bool User::IsValid() const { return 0 != mName[0]; }

        void User::RemoveAccess(uint32_t aFeatures) { mAccess &= ~aFeatures; }

        void User::SetAccess(uint32_t aFeatures) { mAccess = aFeatures; }

        void User::SetName(const char* aName) { strcpy_s(mName, aName); }

        void User::SetPassword(const char* aPassword, uint32_t aSalt)
        {
            mSalt = aSalt;

            mPassword = Hash(aPassword, aSalt);
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        bool User::Load(IFile* aFile)
        {
            auto lRet_byte = aFile->Read(this, sizeof(User));

            bool lResult = sizeof(User) == lRet_byte;
            if (!lResult)
            {
                Clear();
            }

            return lResult;
        }

        bool User::Save(IFile* aFile) const
        {
            auto lRet_byte = aFile->Write(this, sizeof(User));

            return sizeof(User) == lRet_byte;
        }

        bool User::VerifyPassword(const char* aPassword) const
        {
            auto lPassword = Hash(aPassword, mSalt);

            return mPassword == lPassword;
        }

    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

uint64_t Hash(const char* aPassword, uint32_t aSalt)
{
    uint64_t lResult = aSalt;

    for (unsigned int i = 0; i < KMS_EMBEDDED_USER_NAME_LENGTH_MAX; i++)
    {
        if (0 == aPassword[i])
        {
            break;
        }

        lResult += aPassword[i] << 2 * i;

        lResult ^= 0xa596c31122c396a5;
    }

    return lResult;
}
