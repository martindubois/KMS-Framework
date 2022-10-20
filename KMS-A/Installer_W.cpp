
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Installer_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Installer.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void Installer::AddToPath()
    {
        HKEY lKey;

        LSTATUS lRet = RegOpenKeyEx(HKEY_CURRENT_USER, "Environment", 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &lKey);
        KMS_EXCEPTION_ASSERT(ERROR_SUCCESS == lRet, REG_OPEN_FAILED, "Cannot open HKCU\\Environment", lRet);

        char  lData[4096];
        char  lPath[4096 + PATH_LENGTH];
        DWORD lSize_byte = sizeof(lData);
        DWORD lType;

        lRet = RegQueryValueEx(lKey, "PATH", NULL, &lType, reinterpret_cast<LPBYTE>(&lData), &lSize_byte);
        switch (lRet)
        {
        case ERROR_SUCCESS:
            if (NULL != strstr(lData, File::Folder::EXECUTABLE.GetPath()))
            {
                return;
            }

            sprintf_s(lPath, "%s;%s", lData, File::Folder::EXECUTABLE.GetPath());
            break;

        case ERROR_FILE_NOT_FOUND:
            sprintf_s(lPath, "%s", File::Folder::EXECUTABLE.GetPath());
            break;

        default:
            KMS_EXCEPTION(REG_QUERY_FAILED, "Cannot query HKCU\\Environment\\PATH", lRet);
        }

        lRet = RegSetValueEx(lKey, "PATH", NULL, lType, reinterpret_cast<const BYTE*>(lPath), static_cast<DWORD>(strlen(lPath) + 1));
        KMS_EXCEPTION_ASSERT(ERROR_SUCCESS == lRet, REG_SET_FAILED, "Cannot set HKCU\\Environment\\PATH", lRet);
    }

}
