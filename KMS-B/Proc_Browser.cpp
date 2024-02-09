
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Proc_Browser.cpp

// TEST COVERAGE 2022-06-19 Martin Dubois

#include "Component.h"

// ===== C++ ================================================================
#include <thread>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Proc/Process.h>

#include <KMS/Proc/Browser.h>

KMS_RESULT_STATIC(RESULT_START_FAILED);

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_PREFERED (Type::CHROME)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_PREFERED("Prefered = CHROME | DEFAULT | EDGE | FIREFOX | NONE");

namespace KMS
{
    namespace Proc
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Browser::TYPE_NAMES[] = { "CHROME", "DEFAULT", "EDGE", "FIREFOX", "NONE" };

        Browser::Browser()
            : mPrefered(DEFAULT_PREFERED)
            , mAppMode(false)
            , mKioskMode(false)
            , mProcess(nullptr)
            , mType(Type::NONE)
            , mWindow(0)
        {
            AddEntry("Prefered", &mPrefered, false, &MD_PREFERED);
        }

        Browser::~Browser()
        {
            if (nullptr != mProcess)
            {
                Close();

                assert(nullptr == mProcess);
            }
        }

        void Browser::SetAppMode(bool aAM) { mAppMode = aAM; }

        void Browser::SetKioskMode(bool aKM) { mKioskMode = aKM; }

        void Browser::SetPrefered(Type aP) { mPrefered = aP; }

        void Browser::Close()
        {
            Close_OSDep();

            if (nullptr != mProcess)
            {
                if (mProcess->IsRunning())
                {
                    mProcess->Terminate();
                }

                delete mProcess;
                mProcess = nullptr;
            }

            mType   = Type::NONE;
            mWindow = 0;
        }

        void Browser::Detach()
        {
            if (nullptr != mProcess)
            {
                mProcess->Detach();

                delete mProcess;
                mProcess = nullptr;
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
            assert(nullptr != aFile);

            char lURL[PATH_LENGTH];

            sprintf_s(lURL, "http://localhost:%u/%s", aServer.mSocket.GetLocalPort(), aFile);

            Open(lURL);

            RetrieveWindow(aTitle);
        }

        void Browser::Start(const File::Folder& aFolder, const char* aFile)
        {
            Open(aFolder, aFile, nullptr);

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

                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                lTime_ms += 100;
            }

            KMS_EXCEPTION(RESULT_TIMEOUT, "The browser dit not close in allowed time", aTimeout_ms);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Browser::BuildExpectedWindowsTitle(const char* aTitle, char* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aTitle);
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);

            auto lResult = false;

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
            case Type::FIREFOX:
                // TODO  Does not work it the default browser is Chrome
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
            case Type::FIREFOX: CreateProcess_FireFox(aURL); break;

            case Type::NONE: break;

            default: assert(false);
            }
        }

        void Browser::Open(const char* aURL)
        {
            static const Browser::Type ORDER[static_cast<unsigned int>(Browser::Type::QTY)][static_cast<unsigned int>(Browser::Type::QTY) - 1] =
            {
                { Browser::Type::CHROME , Browser::Type::EDGE  , Browser::Type::DEFAULT, Browser::Type::FIREFOX },
                { Browser::Type::DEFAULT, Browser::Type::EDGE  , Browser::Type::CHROME , Browser::Type::FIREFOX },
                { Browser::Type::EDGE   , Browser::Type::CHROME, Browser::Type::DEFAULT, Browser::Type::FIREFOX },
                { Browser::Type::FIREFOX, Browser::Type::CHROME, Browser::Type::DEFAULT, Browser::Type::EDGE    },
                { Browser::Type::NONE }
            };

            KMS_DBG_LOG_INFO();
            Dbg::gLog.WriteMessage(aURL);

            for (unsigned int i = 0; i < static_cast<unsigned int>(Type::NONE) - 1; i++)
            {
                mType = ORDER[static_cast<unsigned int>(static_cast<Type>(mPrefered))][i];

                Create(aURL);
                if (nullptr == mProcess)
                {
                    return;
                }

                Open();

                if (nullptr != mProcess)
                {
                    return;
                }
            }

            KMS_EXCEPTION(RESULT_START_FAILED, "Cannot open a browser (NOT TESTED)", aURL);
        }

        void Browser::Open()
        {
            assert(nullptr != mProcess);

            try
            {
                mProcess->Start();
            }
            catch (Exception eE)
            {
                delete mProcess;
                mProcess = nullptr;

                mType = Type::NONE;

                if (RESULT_START_FAILED != eE.GetCode())
                {
                    throw eE;
                }
            }
        }

    }
}
