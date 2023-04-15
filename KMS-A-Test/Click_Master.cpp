
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Click_Master.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Click/Master.h>
#include <KMS/Proc/Process.h>

using namespace KMS;

KMS_TEST(Click_Master_Base, "Click_Master_Base", "Click_Master", sTest_Base)
{
    Proc::Process lP(File::Folder::NONE, "Notepad.exe");

    lP.Start();
    lP.Detach();

    Click::Master lCM;

    lCM.SetDelay(10);

    unsigned int lFlags = Click::Master::FLAG_NO_EXCEPTION | Click::Master::FLAG_RETRY;
    KMS_TEST_ASSERT_RETURN(lCM.Window_Select(NULL, "Untitled - Notepad", lFlags));

    lCM.SystemKey(Click::Master::KeyCode::F10);
    lCM.Key('F');
    lCM.Key('O');

    lFlags |= Click::Master::FLAG_DESKTOP | Click::Master::FLAG_PUSH;
    KMS_TEST_ASSERT_RETURN(lCM.Window_Select(NULL, "Open", lFlags));
    {
        lFlags = Click::Master::FLAG_NO_EXCEPTION | Click::Master::FLAG_PUSH;
        KMS_TEST_ASSERT_RETURN(lCM.Window_Select("ComboBoxEx32", NULL, lFlags));
        {
            lFlags = Click::Master::FLAG_NO_EXCEPTION;
            KMS_TEST_ASSERT_RETURN(lCM.Window_Select("ComboBox", NULL, lFlags));
            KMS_TEST_ASSERT_RETURN(lCM.Window_Select("Edit"    , NULL, lFlags));

            char lPath[PATH_LENGTH];
            File::Folder::CURRENT.GetPath("KMS-A-Test\\Tests\\Test4.txt", lPath, sizeof(lPath));

            lCM.Text(lPath);
        }
        lCM.Window_Pop();
        KMS_TEST_ASSERT_RETURN(lCM.Window_Select(NULL, "&Open", lFlags));
        lCM.SystemKey('O', Click::Master::FLAG_ALT);
    }
    lCM.Window_Pop();

    lCM.Key(Click::Master::KeyCode::F10);
    lCM.Key('F');
    lCM.Key('x');
}
