
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
            LocateFrontEnd();

            mServer.mOnRequest.Set(this, MSG_ON_REQUEST);
        }

        void ReactApp::AddFunction(const char* aPath, Message::IReceiver* aReceiver, unsigned int aCode)
        {
            mFunctions.insert(FunctionMap::value_type(aPath, Message::Sender(this, aReceiver, aCode)));
        }

        void ReactApp::AddRoute(const char* aPath)
        {
            assert(NULL != aPath);

            mRoutes.insert(aPath);
        }

        // ===== Config::Configurable =======================================

        void ReactApp::InitConfigurator(Config::Configurator* aC)
        {
            mServer.mSocket.InitConfigurator(aC);
        }

        // ===== Message::IReceiver =========================================

        bool ReactApp::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            bool lResult = false;

            switch (aCode)
            {
            case MSG_ON_REQUEST: lResult = OnRequest(aData); break;

            default: assert(false);
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
                File::Folder lProduct(lExec, ".." SLASH "..");

                File::Folder lB(lProduct, "Import" SLASH "front-end");
                if (lB.DoesExist())
                {
                    mFileServer.SetRoot(lB);
                }
                else
                {
                    File::Folder lC(lExec, "front-end" SLASH "build");
                    if (lC.DoesExist())
                    {
                        mFileServer.SetRoot(lB);
                    }
                    else
                    {
                        KMS_EXCEPTION(INSTALLATION, "The front-end files are not available");
                    }
                }
            }
        }

        bool ReactApp::OnFunction(Request* aRequest)
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
                try
                {
                    lIt->second.Send(aRequest);
                }
                catch (...)
                {
                    aRequest->SetResult(Request::Result::INTERNAL_SERVER_ERROR);
                }
            }

            return true;
        }

        bool ReactApp::OnRequest(void* aData)
        {
            assert(NULL != aData);

            Request* lRequest = reinterpret_cast<Request*>(aData);

            const char* lPath = lRequest->GetPath();

            StringSet::iterator lIt = mRoutes.find(lPath);
            if (mRoutes.end() == lIt)
            {
                OnFunction(lRequest);
            }
            else
            {
                mFileServer.ProcessRequest(lRequest, "/index.html");
            }

            return true;
        }

    }
}
