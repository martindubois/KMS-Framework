
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WGDI/Scope.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Scope/Scope.h>
#include <KMS/Thread/Thread.h>
#include <KMS/WGDI/Window_Bitmap.h>

namespace KMS
{
    namespace WGDI
    {

        class Scope final
        {

        public:

            Scope(const char* aTitle);

            // This method set the color of the channel based on the channel
            // index.
            void AddChannel(KMS::Scope::Channel* aChannel);

            void Show();

            void Start();

            void Stop();

            KMS::Scope::Scope mScope;

        private:

            Thread::Thread    mScope_Thread;
            Window_Bitmap     mWindow;
            Thread::Thread    mWindow_Thread;

        };

    }
}
