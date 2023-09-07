
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WGDI/Window.h

#pragma once

// ===== C++ ================================================================
#include <string>

namespace KMS
{
    namespace WGDI
    {

        class Window
        {

        public:

            Window();

            virtual ~Window();

            void SetTitle(const char* aTitle);

            virtual void Show();

            // ===== Configurable attribute =================================
            uint16_t    mSizeX_px;
            uint16_t    mSizeY_px;
            std::string mTitle;

        // Internal

            virtual LRESULT WindowProc(UINT aMsg, WPARAM aParamW, LPARAM aParamL);

        protected:

            HWND GetWindow();

            void Create();

            void Run();

            virtual void Paint(HDC aDC);

            virtual void ValidateConfig();

            virtual LRESULT On_WM_PAINT();

        private:

            NO_COPY(Window);

            HINSTANCE mInstance;
            HWND      mWindow;

        };

    }
}