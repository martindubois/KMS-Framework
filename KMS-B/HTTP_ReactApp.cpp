
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/HTTP/Transaction.h>

#include <KMS/HTTP/ReactApp.h>

KMS_RESULT_STATIC(RESULT_INSTALLATION_ERROR);

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        ReactApp::ReactApp()
            // ===== Callbacks ==============================================
            : ON_REQUEST(this, &ReactApp::OnRequest)
        {
            AddEntry("FileServer", &mFileServer, false);

            LocateFrontEnd();

            mServer.mOnRequest = &ON_REQUEST;
        }

        void ReactApp::AddFunction(const char* aPath, const ICallback* aCallback)
        {
            mFunctions.insert(FunctionMap::value_type(aPath, const_cast<ICallback*>(aCallback)));
        }

        void ReactApp::AddRoute(const char* aPath)
        {
            assert(nullptr != aPath);

            mRoutes.insert(aPath);
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
                    KMS_EXCEPTION_ASSERT(lC.DoesExist(), RESULT_INSTALLATION_ERROR, "The front-end files are not available", lC.GetPath());

                    mFileServer.SetRoot(lB);
                }
            }
        }

        unsigned int ReactApp::OnFunction(Transaction* aTransaction)
        {
            assert(nullptr != aTransaction);

            auto lPath = aTransaction->GetPath();

            auto lIt = mFunctions.find(lPath);
            if (mFunctions.end() == lIt)
            {
                mFileServer.ProcessRequest(aTransaction);
            }
            else
            {
                auto lRet = lIt->second->Send(this, aTransaction);
                if (!CALLBACK_SUCCESS(lRet))
                {
                    aTransaction->SetResult(Result::INTERNAL_SERVER_ERROR);
                }
            }

            return 0;
        }

        // ===== Callbacks ==================================================

        unsigned int ReactApp::OnRequest(void*, void* aData)
        {
            assert(nullptr != aData);

            auto lTransaction = reinterpret_cast<Transaction*>(aData);

            auto lPath = lTransaction->GetPath();

            auto lIt = mRoutes.find(lPath);
            if (mRoutes.end() == lIt)
            {
                OnFunction(lTransaction);
            }
            else
            {
                mFileServer.ProcessRequest(lTransaction, "/index.html");
            }

            return 0;
        }

    }
}
