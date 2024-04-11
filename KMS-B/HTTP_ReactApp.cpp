
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/HTTP/HTTP.h>
#include <KMS/HTTP/Transaction.h>

#include <KMS/HTTP/ReactApp.h>

KMS_RESULT_STATIC(RESULT_INSTALLATION_ERROR);

// Constantes
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_ROUTES("Routes += {RelPath}");

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* ReactApp::DEFAULT_ROUTE = "/";

        ReactApp::ReactApp()
            : ON_REQUEST(this, &ReactApp::OnRequest)
        {
            mRoutes.SetCreator(DI::String_Expand::Create);

            Ptr_OF < DI::Object> lEntry;

            lEntry.Set(new DI::String_Expand(DEFAULT_ROUTE), true); mRoutes.AddEntry(lEntry);

            lEntry.Set(&mFileServer, false); AddEntry("FileServer", lEntry);
            lEntry.Set(&mRoutes    , false); AddEntry("Routes"    , lEntry, &MD_ROUTES);

            LocateFrontEnd();

            mServer.mOnRequest = &ON_REQUEST;
        }

        void ReactApp::AddFunction(const char* aPath, const ICallback* aCallback)
        {
            mFunctions.insert(FunctionMap::value_type(aPath, const_cast<ICallback*>(aCallback)));
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
                    File::Folder lC(lProduct, "front-end" SLASH "build");
                    if (lC.DoesExist())
                    {
                        mFileServer.SetRoot(lB);
                    }
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
                Ptr_OF<DI::Object> lEntry(&Response::FIELD_VALUE_ACCESS_CONTROL_ALLOW_HEADERS_DEFAULT);

                switch (aTransaction->GetType())
                {
                case Transaction::Type::OPTIONS:
                    aTransaction->mResponse_Header.AddEntry(Response::FIELD_NAME_ACCESS_CONTROL_ALLOW_HEADERS, lEntry);
                    break;

                default:
                    auto lRet = lIt->second->Send(this, aTransaction);
                    if (!CALLBACK_SUCCESS(lRet))
                    {
                        aTransaction->SetResult(Result::INTERNAL_SERVER_ERROR);
                    }
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

            for (const auto& lEntry : mRoutes.mInternal)
            {
                auto lRoute = dynamic_cast<const DI::String_Expand*>(lEntry.Get());
                if (*lRoute == lPath)
                {
                    mFileServer.ProcessRequest(lTransaction, "/index.html");
                    return 0;
                }
            }

            OnFunction(lTransaction);
            return 0;
        }

    }
}
