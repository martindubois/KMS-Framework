
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WGDI_Window.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/WGDI/Window.h>

KMS_RESULT_STATIC(RESULT_CREATE_FAILED);

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CLASS_NAME "KMS-WGDI"

#define DEFAULT_TITLE "KMS-WGDI"

#define FLAGS (SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOZORDER)

#define STYLES (WS_CAPTION | WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE)

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static LRESULT CALLBACK WindowProc_Link(HWND aWindow, UINT aMsg, WPARAM aParamW, LPARAM aParamL);

namespace KMS
{
    namespace WGDI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Window::Window()
            : mInstance(GetModuleHandle(NULL))
            , mWindow(nullptr)
            // ===== Configurable attributes ================================
            , mTitle(DEFAULT_TITLE)
        {
            assert(nullptr != mInstance);

            WNDCLASSEX lClass;

            memset(&lClass, 0, sizeof(lClass));

            lClass.cbSize        = sizeof(lClass);
            lClass.cbWndExtra    = sizeof(Window*);
            lClass.hInstance     = mInstance;
            lClass.lpfnWndProc   = WindowProc_Link;
            lClass.lpszClassName = CLASS_NAME;
            lClass.style         = CS_GLOBALCLASS | CS_OWNDC;

            auto lAtom = RegisterClassEx(&lClass);
            assert(0 != lAtom);
        }

        Window::~Window()
        {
            assert(nullptr != mInstance);

            BOOL lRet;

            if (nullptr != mWindow)
            {
                lRet = DestroyWindow(mWindow);
                assert(lRet);
            }

            lRet = UnregisterClass(CLASS_NAME, mInstance);
            assert(lRet);
        }

        void Window::Show()
        {
            ValidateConfig();

            Create();

            Run();
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        LRESULT Window::WindowProc(UINT aMsg, WPARAM aParamW, LPARAM aParamL)
        {
            assert(nullptr != mWindow);

            LRESULT lResult;

            switch (aMsg)
            {
            case WM_PAINT: lResult = On_WM_PAINT(); break;

            default: lResult = DefWindowProc(mWindow, aMsg, aParamW, aParamL);
            }

            return lResult;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        HWND Window::GetWindow() { return mWindow; }

        void Window::Create()
        {
            assert(nullptr == mWindow);

            mWindow = CreateWindowEx(WS_EX_APPWINDOW, CLASS_NAME, mTitle.c_str(), STYLES, CW_USEDEFAULT, CW_USEDEFAULT, mSizeX_px, mSizeY_px, NULL, NULL, mInstance, NULL);
            KMS_EXCEPTION_ASSERT(nullptr != mWindow, RESULT_CREATE_FAILED, "Cannot create window", "");

            auto lRet = SetWindowLongPtr(mWindow, 0, reinterpret_cast<ULONG_PTR>(this));
            assert(0 == lRet);

            RECT lRect;

            auto lRetB = GetClientRect(mWindow, &lRect);
            KMS_EXCEPTION_ASSERT(lRetB, RESULT_CREATE_FAILED, "Cannot create window", "");

            // When we first create the windows, the size we indicated
            // include the borders. We need to adjust size to get the size we
            // want for the client area.
            //               Client                  Old size
            //   New size    area        Border size -------------------------------
            //   =========   =========   ===========================================
            auto lSizeX_px = mSizeX_px + mSizeX_px - (lRect.right - lRect.left + 1);
            auto lSizeY_px = mSizeY_px + mSizeY_px - (lRect.bottom - lRect.top + 1);

            lRetB = SetWindowPos(mWindow, nullptr, 0, 0, lSizeX_px, lSizeY_px, FLAGS);
            KMS_EXCEPTION_ASSERT(lRetB, RESULT_CREATE_FAILED, "Cannot create window", "");
        }

        void Window::Run()
        {
            MSG lMsg;

            for (;;)
            {
                if (!IsWindow(mWindow))
                {
                    mWindow = nullptr;
                    break;
                }

                if (0 >= GetMessage(&lMsg, nullptr, 0, 0))
                {
                    break;
                }

                TranslateMessage(&lMsg);
                DispatchMessage (&lMsg);
            }
        }

        void Window::Paint(HDC) {}

        void Window::ValidateConfig()
        {
            KMS_EXCEPTION_ASSERT(0 < mSizeX_px    , RESULT_INVALID_CONFIG, "Invalid size X", "");
            KMS_EXCEPTION_ASSERT(0 < mSizeY_px    , RESULT_INVALID_CONFIG, "Invalid size Y", "");
            KMS_EXCEPTION_ASSERT(0 < mTitle.size(), RESULT_INVALID_CONFIG, "Invalid window title", "");
        }

        LRESULT Window::On_WM_PAINT()
        {
            assert(nullptr != mWindow);

            PAINTSTRUCT lPaint;

            auto lDC = BeginPaint(mWindow, &lPaint);
            {
                Paint(lDC);
            }
            EndPaint(mWindow, &lPaint);

            return 0;
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WindowProc_Link(HWND aWindow, UINT aMsg, WPARAM aParamW, LPARAM aParamL)
{
    assert(nullptr != aWindow);

    LRESULT lResult;

    auto lThis = reinterpret_cast<WGDI::Window*>(GetWindowLongPtr(aWindow, 0));
    if (NULL != lThis)
    {
        lResult = lThis->WindowProc(aMsg, aParamW, aParamL);
    }
    else
    {
        lResult = DefWindowProc(aWindow, aMsg, aParamW, aParamL);
    }

    return lResult;
}
