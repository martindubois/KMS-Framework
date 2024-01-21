
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Proc_Browser_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

#include <KMS/Proc/Browser.h>

namespace KMS
{
    namespace Proc
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool Browser::IsOpen() const
        {
            if (0 != mWindow)
            {
                return IsWindow(reinterpret_cast<HWND>(mWindow));
            }

            return (nullptr != mProcess) && mProcess->IsRunning();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Browser::Close_OSDep()
        {
            if (0 != mWindow)
            {
                assert(nullptr != mProcess);

                if (PostMessage(reinterpret_cast<HWND>(mWindow), WM_CLOSE, 0, 0))
                {
                    mProcess->Wait(1000 * 2);
                }
            }
        }

        void Browser::CreateProcess_Chrome(const char* aURL)
        {
            assert(nullptr == mProcess);

            File::Folder lPF(File::Folder::Id::PROGRAM_FILES);

            mProcess = new Process(File::Folder(lPF, "Google\\Chrome\\Application"), "chrome.exe");

            mProcess->AddArgument("--disable-background-mode");
            mProcess->AddArgument("--disable-plugins");
            mProcess->AddArgument("--start-maximized");

            if (mKioskMode)
            {
                mProcess->AddArgument("--kiosk");
                mProcess->AddArgument(aURL);
            }
            else if (mAppMode)
            {
                char lApp[NAME_LENGTH];

                sprintf_s(lApp, "--app=%s", aURL);

                mProcess->AddArgument(lApp);
            }
            else
            {
                mProcess->AddArgument("--new-windows");
                mProcess->AddArgument(aURL);
            }
        }

        void Browser::CreateProcess_Default(const char* aURL)
        {
            assert(nullptr == mProcess);

            mProcess = new Process(File::Folder(File::Folder::Id::NONE), aURL);

            mProcess->SetVerb("open");
        }

        void Browser::CreateProcess_Edge(const char* aURL)
        {
            assert(nullptr == mProcess);

            File::Folder lPF(File::Folder::Id::PROGRAM_FILES_X86);

            mProcess = new Process(File::Folder(lPF, "Microsoft\\Edge\\Application"), "msedge.exe");

            if (mKioskMode)
            {
                // NOT TESTED
                mProcess->AddArgument("--kiosk");
                mProcess->AddArgument(aURL);
                mProcess->AddArgument("--edge-kiosk-type=fullscreen");
                mProcess->AddArgument("--no-first-run");
            }
            else
            {
                mProcess->AddArgument(aURL);
                mProcess->AddArgument("--new-window");
                mProcess->AddArgument("--no-first-run");
            }
        }

        // TODO  FireFox support on Windows
        void Browser::CreateProcess_FireFox(const char* aURL)
        {
            CreateProcess_Chrome(aURL);
        }

        void Browser::RetrieveWindow(const char* aTitle)
        {
            assert(0 == mWindow);

            char lTitle[NAME_LENGTH];

            if ((nullptr != aTitle) && BuildExpectedWindowsTitle(aTitle, lTitle, sizeof(lTitle)))
            {
                for (unsigned int i = 0; (0 == mWindow) && (i < 20); i++)
                {
                    Sleep(500);

                    mWindow = reinterpret_cast<uint64_t>(FindWindow(NULL, lTitle));
                }
            }
        }

    }
}
