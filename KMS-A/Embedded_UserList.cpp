
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Embedded.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/IFile.h>
#include <KMS/Embedded/User.h>

#include <KMS/Embedded/UserList.h>

namespace KMS
{
    namespace Embedded
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        UserList::UserList(User* aVector, uint8_t aLength)
            : mCurrent(nullptr), mVector(aVector), mVectorLength(aLength)
        {}

        const User* UserList::GetCurrentUser() const { return mCurrent; }

        User* UserList::GetUser(uint8_t aIndex)
        {
            unsigned int lIndex = 0;

            for (uint8_t i = 0; i < mVectorLength; i++)
            {
                if (mVector[i].IsValid())
                {
                    if (aIndex == lIndex)
                    {
                        return mVector + i;
                    }

                    lIndex++;
                }
            }

            return nullptr;
        }

        User* UserList::GetUser(const char* aName)
        {
            for (uint8_t i = 0; i < mVectorLength; i++)
            {
                if (mVector[i].Is(aName))
                {
                    return mVector + i;
                }
            }

            return nullptr;
        }

        uint8_t UserList::GetUserCount() const
        {
            uint8_t lResult = 0;

            for (uint8_t i = 0; i < mVectorLength; i++)
            {
                if (mVector[i].IsValid())
                {
                    lResult++;
                }
            }

            return lResult;
        }

        bool UserList::IsAllowed(uint32_t aFeatures) const
        {
            return (nullptr != mCurrent) && mCurrent->IsAllowed(aFeatures);
        }

        void UserList::AutoLogon(IFile* aFile)
        {
            char lBuffer[36];

            auto lRet_byte = aFile->Read(lBuffer, sizeof(lBuffer));

            if (sizeof(lBuffer) > lRet_byte)
            {
                lBuffer[lRet_byte] = '\0';
            }

            bool lFlag = false;

            const char* lPassword = nullptr;

            for (unsigned int i = 0; i < lRet_byte; i++)
            {
                if ('\0' == lBuffer[i])
                {
                    break;
                }

                switch (lBuffer[i])
                {
                case '\n':
                case '\r':
                case '\t':
                    lBuffer[i] = '\0';
                    lFlag = true;
                    break;

                default:
                    if (lFlag && (nullptr == lPassword))
                    {
                        lPassword = lBuffer + i;
                    }
                }
            }

            if (nullptr != lPassword)
            {
                Login(lBuffer, lPassword);
            }
        }

        void UserList::Login(const char* aName, const char* aPassword)
        {
            Logout();

            auto lUser = GetUser(aName);
            if ((nullptr != lUser) && lUser->VerifyPassword(aPassword))
            {
                mCurrent = lUser;
            }
        }

        void UserList::Logout() { mCurrent = nullptr; }

        void UserList::Load(IFile* aFile)
        {
            for (uint8_t i = 0; i < mVectorLength; i++)
            {
                if (!mVector[i].Load(aFile))
                {
                    break;
                }
            }
        }

        bool UserList::Save(IFile* aFile) const
        {
            bool lResult = true;

            for (uint8_t i = 0; i < mVectorLength; i++)
            {
                if (mVector[i].IsValid())
                {
                    lResult = mVector[i].Save(aFile);
                    if (!lResult)
                    {
                        break;
                    }
                }
            }

            return lResult;
        }

    }
}
