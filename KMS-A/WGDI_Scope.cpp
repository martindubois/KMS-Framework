
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WGDI_Scope.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Scope/Channel.h>
#include <KMS/WGDI/Scope.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define SIZE_X_px (512)
#define SIZE_Y_px (512)

namespace KMS
{
    namespace WGDI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Scope::Scope(const char* aTitle)
        {
            mScope_Thread .mOnIterate  = &mScope .ON_ITERATE;
            mWindow_Thread.mOnIterate  = &mWindow.ON_ITERATE;
            mWindow_Thread.mOnStarting = &mWindow.ON_STARTING;
            mWindow_Thread.mOnStopping = &mWindow.ON_STOPPING;

            mScope.mBitmap.Init(SIZE_X_px, SIZE_Y_px);

            mWindow.mPeriod_ms = 10;

            mWindow.SetBitmap(&mScope.mBitmap);
            mWindow.SetTitle(aTitle);
        }

        void Scope::AddChannel(KMS::Scope::Channel* aChannel)
        {
            assert(nullptr != aChannel);

            switch (mScope.Channels_GetCount() % 4)
            {
            case 0: aChannel->mColor = Graph::Color::RED  ; break;
            case 1: aChannel->mColor = Graph::Color::GREEN; break;
            case 2: aChannel->mColor = Graph::Color::BLUE ; break;
            case 3: aChannel->mColor = Graph::Color::WHITE; break;
            }

            mScope.Channel_Add(aChannel);
        }

        void Scope::Show() { mWindow_Thread.Start(); }

        void Scope::Start()
        {
            mScope.Start();

            mScope_Thread.Start();
        }

        void Scope::Stop()
        {
            mScope.Stop();

            mScope_Thread.StopAndWait(2000);
        }

    }
}
