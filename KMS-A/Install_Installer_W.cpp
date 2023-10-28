
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Installer_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Install/Installer.h>

KMS_RESULT_STATIC(RESULT_REG_OPEN_FAILED);
KMS_RESULT_STATIC(RESULT_REG_QUERY_FAILED);
KMS_RESULT_STATIC(RESULT_REG_SET_FAILED);

// Constants
// //////////////////////////////////////////////////////////////////////////

#define REG_KEY_NAME   "Environment"
#define REG_VALUE_NAME "PATH"

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CloseKey(HKEY aKey);
static HKEY OpenKey();
static void SetValue(HKEY aKey, DWORD aType, const char* aIn);

static void Error_CannotQuery(HKEY aKey);

namespace KMS
{
    namespace Install
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Installer::AddToPath()
        {
            auto lKey = OpenKey();

            char  lData[4096];
            auto  lFolder = File::Folder::EXECUTABLE.GetPath();
            char  lPath[4096 + PATH_LENGTH];
            DWORD lSize_byte = sizeof(lData);
            DWORD lType;

            auto lRet = RegQueryValueEx(lKey, REG_VALUE_NAME, nullptr, &lType, reinterpret_cast<LPBYTE>(&lData), &lSize_byte);
            switch (lRet)
            {
            case ERROR_SUCCESS:
                if (NULL != strstr(lData, lFolder))
                {
                    CloseKey(lKey);
                    return;
                }

                sprintf_s(lPath, "%s;%s", lData, lFolder);
                break;

            case ERROR_FILE_NOT_FOUND:
                sprintf_s(lPath, "%s", lFolder);
                break;

            default: Error_CannotQuery(lKey);
            }

            SetValue(lKey, lType, lPath);
        }

        void Installer::RemoveFromPath()
        {
            auto lKey = OpenKey();

            char  lData[4096];
            auto  lFolder = File::Folder::EXECUTABLE.GetPath();
            char* lPtr;
            DWORD lSize_byte = sizeof(lData);
            DWORD lType;

            auto lRet = RegQueryValueEx(lKey, REG_VALUE_NAME, nullptr, &lType, reinterpret_cast<LPBYTE>(&lData), &lSize_byte);
            switch (lRet)
            {
            case ERROR_SUCCESS:
                lPtr = strstr(lData, lFolder);
                if (nullptr != lPtr)
                {
                    auto lFolderLen = strlen(lFolder);
                    auto lPtrLen    = strlen(lPtr);

                    if (';' == lPtr[lFolderLen])
                    {
                        lFolderLen++;
                    }

                    memmove(lPtr, lPtr + lFolderLen, lPtrLen - lFolderLen);

                    SetValue(lKey, lType, lData);
                }
                // No break

            case ERROR_FILE_NOT_FOUND:
                CloseKey(lKey);
                break;

            default: Error_CannotQuery(lKey);
            }

        }

    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void CloseKey(HKEY aKey)
{
    auto lRet = RegCloseKey(aKey);
    assert(ERROR_SUCCESS == lRet);
}

HKEY OpenKey()
{
    HKEY lResult;

    auto lRet = RegOpenKeyEx(HKEY_CURRENT_USER, REG_KEY_NAME, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &lResult);
    KMS_EXCEPTION_ASSERT(ERROR_SUCCESS == lRet, RESULT_REG_OPEN_FAILED, "Cannot open HKCU\\" REG_KEY_NAME, lRet);

    return lResult;
}

void SetValue(HKEY aKey, DWORD aType, const char* aIn)
{
    assert(nullptr != aIn);

    auto lRet = RegSetValueEx(aKey, REG_VALUE_NAME, 0, aType, reinterpret_cast<const BYTE*>(aIn), static_cast<DWORD>(strlen(aIn) + 1));

    CloseKey(aKey);

    KMS_EXCEPTION_ASSERT(ERROR_SUCCESS == lRet, RESULT_REG_SET_FAILED, "Cannot set HKCU\\" REG_KEY_NAME "\\" REG_VALUE_NAME, lRet);
}

void Error_CannotQuery(HKEY aKey)
{
    CloseKey(aKey);

    KMS_EXCEPTION(RESULT_REG_QUERY_FAILED, "Cannot query HKCU\\" REG_KEY_NAME "\\" REG_VALUE_NAME, "");
}
