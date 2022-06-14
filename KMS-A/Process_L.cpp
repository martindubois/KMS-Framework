
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Process_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== Includes ===========================================================
#include <KMS/Process.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void ChangeDirectory(const char* aD);

namespace KMS
{

    // Pirvate
    // //////////////////////////////////////////////////////////////////////

    int Process::Run_Internal()
    {
        assert(NULL != mCmdLine);

        bool lCD = 0 < mWorkingDirectory.size();

        char lDir[MAX_PATH];

        if (lCD)
        {
            if (NULL == getcwd(lDir, sizeof(lDir)))
            {
                KMS_EXCEPTION(FOLDER_ACCESS, "getcwd failed");
            }

            ChangeDirectory(mWorkingDirectory.c_str());
        }

        int lResult = system(mCmdLine);

        if (lCD)
        {
            ChangeDirectory(lDir);
        }

        return lResult;
    }

}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void ChangeDirectory(const char* aD)
{
    assert(NULL != aD);

    if (0 != chdir(aD))
    {
        KMS_EXCEPTION_WITH_INFO(FOLDER_ACCESS, "chdir failed", aD);
    }
}
