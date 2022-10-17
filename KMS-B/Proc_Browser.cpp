
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Proc_Browser.cpp

// TEST COVERAGE 2022-06-19 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Proc/Process.h>

#include <KMS/Proc/Browser.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_PREFERED (Type::CHROME)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_PREFERED("Prefered = CHROME | DEFAULT | EDGE | NONE");

namespace KMS
{
    namespace Proc
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Browser::TYPE_NAMES[] = { "CHROME", "DEFAULT", "EDGE", "NONE" };

        Browser::Browser()
            : mPrefered(DEFAULT_PREFERED)
            , mAppMode(false)
            , mKioskMode(false)
            , mProcess(NULL)
            , mType(Type::NONE)
            , mWindow(0)
        {
            AddEntry("Prefered", &mPrefered, false, &MD_PREFERED);
        }

        Browser::~Browser()
        {
            if (NULL != mProcess)
            {
                Close();

                assert(NULL == mProcess);
            }
        }

        bool Browser::IsOpen() const
        {
            if (0 != mWindow)
            {
                return IsWindow(reinterpret_cast<HWND>(mWindow));
            }

            return (NULL != mProcess) && mProcess->IsRunning();
        }

        void Browser::SetAppMode(bool aAM) { mAppMode = aAM; }

        void Browser::SetKioskMode(bool aKM) { mKioskMode = aKM; }

        void Browser::SetPrefered(Type aP) { mPrefered = aP; }

        void Browser::Close()
        {
            if (0 != mWindow)
            {
                assert(NULL != mProcess);

                if (PostMessage(reinterpret_cast<HWND>(mWindow), WM_CLOSE, 0, 0))
                {
                    mProcess->Wait(1000 * 2);
                }
            }

            if (NULL != mProcess)
            {
                if (mProcess->IsRunning())
                {
                    mProcess->Terminate();
                }

                delete mProcess;
                mProcess = NULL;
            }

            mType   = Type::NONE;
            mWindow = 0;
        }

        void Browser::Detach()
        {
            if (NULL != mProcess)
            {
                mProcess->Detach();

                delete mProcess;
                mProcess = NULL;
            }

            mType   = Type::NONE;
            mWindow = 0;
        }

        void Browser::Open(const File::Folder& aFolder, const char* aFile, const char* aTitle)
        {
            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            Open(lPath);

            RetrieveWindow(aTitle);
        }

        void Browser::Open(const HTTP::Server& aServer, const char* aFile, const char* aTitle)
        {
            assert(NULL != aFile);

            char lURL[PATH_LENGTH];

            sprintf_s(lURL, "http://localhost:%u/%s", aServer.mSocket.GetLocalPort(), aFile);

            Open(lURL);

            RetrieveWindow(aTitle);
        }

        void Browser::Start(const File::Folder& aFolder, const char* aFile)
        {
            Open(aFolder, aFile, NULL);

            Detach();
        }

        void Browser::Wait(unsigned int aTimeout_ms)
        {
            unsigned int lTime_ms = 0;

            while (aTimeout_ms >= lTime_ms)
            {
                if (!IsOpen())
                {
                    return;
                }

                Sleep(100); // ms
                lTime_ms += 100;
            }

            KMS_EXCEPTION(PROC_TIMEOUT, "The browser dit not close in allowed time", aTimeout_ms);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Browser::BuildExpectedWindowsTitle(const char* aTitle, char* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aTitle);
            assert(NULL != aOut);
            assert(0 < aOutSize_byte);

            bool lResult = false;

            switch (mType)
            {
            case Type::CHROME:
                if (!mKioskMode)
                {
                    if (mAppMode)
                    {
                        sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s", aTitle);
                    }
                    else
                    {
                        sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s - Google Chrome", aTitle);
                    }
                    lResult = true;
                }
                break;

            case Type::DEFAULT:
                // TODO Does not work it the default browser is Chrome
                sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s", aTitle);
                lResult = true;
                break;

            case Type::EDGE:
                if (!mKioskMode)
                {
                    sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s", aTitle);
                    lResult = true;
                }
                break;

            case Type::NONE: break;

            default: assert(false);
            }

            return lResult;
        }

        void Browser::Create(const char* aURL)
        {
            switch (mType)
            {
            case Type::CHROME : CreateProcess_Chrome (aURL); break;
            case Type::DEFAULT: CreateProcess_Default(aURL); break;
            case Type::EDGE   : CreateProcess_Edge   (aURL); break;

            case Type::NONE: break;

            default: assert(false);
            }
        }

        void Browser::CreateProcess_Chrome(const char* aURL)
        {
            assert(NULL == mProcess);

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
            assert(NULL == mProcess);

            mProcess = new Process(File::Folder(File::Folder::Id::NONE), aURL);

            mProcess->SetVerb("open");
        }

        void Browser::CreateProcess_Edge(const char* aURL)
        {
            assert(NULL == mProcess);

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

        void Browser::RetrieveWindow(const char* aTitle)
        {
            assert(0 == mWindow);

            char lTitle[NAME_LENGTH];

            if ((NULL != aTitle) && BuildExpectedWindowsTitle(aTitle, lTitle, sizeof(lTitle)))
            {
                for (unsigned int i = 0; (0 == mWindow) && (i < 20); i++)
                {
                    Sleep(500);

                    mWindow = reinterpret_cast<uint64_t>(FindWindow(NULL, lTitle));
                }
            }
        }

        void Browser::Open(const char* aURL)
        {
            static const Browser::Type ORDER[static_cast<unsigned int>(Browser::Type::QTY)][static_cast<unsigned int>(Browser::Type::QTY) - 1] =
            {
                { Browser::Type::CHROME , Browser::Type::EDGE  , Browser::Type::DEFAULT },
                { Browser::Type::DEFAULT, Browser::Type::EDGE  , Browser::Type::CHROME  },
                { Browser::Type::EDGE   , Browser::Type::CHROME, Browser::Type::DEFAULT },
                { Browser::Type::NONE }
            };

            for (unsigned int i = 0; i < static_cast<unsigned int>(Type::NONE) - 1; i++)
            {
                mType = ORDER[static_cast<unsigned int>(static_cast<Type>(mPrefered))][i];

                Create(aURL);
                if (NULL == mProcess)
                {
                    return;
                }

                Open();

                if (NULL != mProcess)
                {
                    return;
                }
            }

            // NOT TESTED
            KMS_EXCEPTION(PROC_START_FAILED, "Cannot open a browser", aURL);
        }

        void Browser::Open()
        {
            assert(NULL != mProcess);

            try
            {
                mProcess->Start();
            }
            catch (Exception eE)
            {
                delete mProcess;
                mProcess = NULL;

                mType = Type::NONE;

                switch (eE.GetCode())
                {
                case Exception::Code::PROC_START_FAILED: break;

                default: throw eE;
                }
            }
        }

    }
}
