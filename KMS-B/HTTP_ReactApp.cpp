
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/HTTP/Request.h>

#include <KMS/HTTP/ReactApp.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_ON_REQUEST (1)

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        ReactApp::ReactApp()
        {
            AddEntry("FileServer", &mFileServer);

            LocateFrontEnd();

            mServer.mOnRequest.Set(this, MSG_ON_REQUEST);
        }

        void ReactApp::AddFunction(const char* aPath, Msg::IReceiver* aReceiver, unsigned int aCode)
        {
            mFunctions.insert(FunctionMap::value_type(aPath, Msg::Destination(aReceiver, aCode)));
        }

        void ReactApp::AddRoute(const char* aPath)
        {
            assert(NULL != aPath);

            mRoutes.insert(aPath);
        }

        // ===== Msg::IReceiver =============================================

        unsigned int ReactApp::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult;

            switch (aCode)
            {
            case MSG_ON_REQUEST: lResult = OnRequest(aData); break;

            default:
                assert(false);
                lResult = Msg::IReceiver::MSG_IGNORED;
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void ReactApp::LocateFrontEnd()
        {
            File::Folder lExec(File::Folder::Id::EXECUTABLE);

            File::Folder lA(lExec, "front-end");
            if (lA.DoesExist())
            {
                mFileServer.SetRoot(lA);
            }
            else
            {
                #if defined(_KMS_WINDOWS_) && ! defined(_WIN64)
                    File::Folder lProduct(lExec, "..");
                #else
                    File::Folder lProduct(lExec, ".." SLASH "..");
                #endif

                File::Folder lB(lProduct, "Import" SLASH "front-end");
                if (lB.DoesExist())
                {
                    mFileServer.SetRoot(lB);
                }
                else
                {
                    File::Folder lC(lExec, "front-end" SLASH "build");
                    KMS_EXCEPTION_ASSERT(lC.DoesExist(), HTTP_INSTALLATION_ERROR, "The front-end files are not available", lC.GetPath());

                    mFileServer.SetRoot(lB);
                }
            }
        }

        unsigned int ReactApp::OnFunction(Request* aRequest)
        {
            assert(NULL != aRequest);

            const char* lPath = aRequest->GetPath();

            FunctionMap::iterator lIt = mFunctions.find(lPath);
            if (mFunctions.end() == lIt)
            {
                mFileServer.ProcessRequest(aRequest);
            }
            else
            {
                unsigned int lRet = lIt->second.Send(this, aRequest);
                if (!KMS_MSG_SUCCESS(lRet))
                {
                    aRequest->SetResult(Request::Result::INTERNAL_SERVER_ERROR);
                }
            }

            return 0;
        }

        unsigned int ReactApp::OnRequest(void* aData)
        {
            assert(NULL != aData);

            Request* lRequest = reinterpret_cast<Request*>(aData);

            const char* lPath = lRequest->GetPath();

            StringSet_ASCII::iterator lIt = mRoutes.find(lPath);
            if (mRoutes.end() == lIt)
            {
                OnFunction(lRequest);
            }
            else
            {
                mFileServer.ProcessRequest(lRequest, "/index.html");
            }

            return 0;
        }

    }
}
