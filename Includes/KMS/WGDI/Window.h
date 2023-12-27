
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WGDI/Window.h

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/Callback.h>

namespace KMS
{
    namespace WGDI
    {

        class Window
        {

        public:

            const Callback<Window> ON_ITERATE;
            const Callback<Window> ON_STARTING;
            const Callback<Window> ON_STOPPING;

            Window();

            virtual ~Window();

            void SetTitle(const char* aTitle);

            // Call this method if the program use the main thread to create
            // the windows and process message. If it uses a dedicated
            // thread, connect the thread to ON_ITERATE, ON_STARTING and
            // ON_STOPPING. This method returns when the user close the
            // window.
            void Run();

            // ===== Configurable attribute =================================
            uint16_t    mSizeX_px;
            uint16_t    mSizeY_px;
            std::string mTitle;

        // Internal

            virtual LRESULT WindowProc(UINT aMsg, WPARAM aParamW, LPARAM aParamL);

        protected:

            HWND GetWindow();

            void Create();

            virtual void Paint(HDC aDC);

            virtual void ValidateConfig();

            virtual LRESULT On_WM_PAINT();

            virtual unsigned int OnStarting(void* aSender, void* aData);

        private:

            NO_COPY(Window);

            unsigned int OnIterate (void* aSender, void* aData);
            unsigned int OnStopping(void* aSender, void* aData);

            HINSTANCE mInstance;
            HWND      mWindow;

        };

    }
}