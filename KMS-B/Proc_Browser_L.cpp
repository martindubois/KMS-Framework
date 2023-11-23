
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Proc_Browser_L.cpp

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
            return (nullptr != mProcess) && mProcess->IsRunning();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Browser::Close_OSDep() {}

        void Browser::CreateProcess_Chrome(const char* aURL)
        {
            assert(nullptr == mProcess);

            mProcess = new Process(File::Folder("/opt/google/chrome"), "chrome");

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

            mProcess = new Process(File::Folder("/bin"), "xdg-open");

            mProcess->AddArgument(aURL);
        }

        // TODO  Edge support on Linux
        void Browser::CreateProcess_Edge(const char* aURL)
        {
            CreateProcess_Chrome(aURL);
        }

        void Browser::CreateProcess_FireFox(const char* aURL)
        {
            assert(nullptr == mProcess);

            mProcess = new Process(File::Folder("/snap/firefox/3358/usr/lib/firefox"), "firefox");

            mProcess->AddArgument("-foreground");

            if (mKioskMode)
            {
                mProcess->AddArgument("--kiosk");
                mProcess->AddArgument(aURL);
            }
            else
            {
                mProcess->AddArgument("-new-instance");
                mProcess->AddArgument(aURL);
            }
        }

        void Browser::RetrieveWindow(const char* aTitle)
        {
            assert(0 == mWindow);
        }

    }
}
