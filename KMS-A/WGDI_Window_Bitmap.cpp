
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WGDI_Window_Bitmap.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/WGDI/Window_Bitmap.h>

// Configurations
// //////////////////////////////////////////////////////////////////////////

#define PERIOD_DISABLE (0)

#define DEFAULT_PERIOD_ms PERIOD_DISABLE
#define DEFAULT_ZOOM      (  1)

// Constants
// //////////////////////////////////////////////////////////////////////////

namespace KMS
{
    namespace WGDI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Window_Bitmap::Window_Bitmap()
            // ===== Configurable attributes ================================
            : mPeriod_ms(DEFAULT_PERIOD_ms)
            , mZoom     (DEFAULT_ZOOM)
        {
            assert(sizeof(BITMAPINFO) <= sizeof(mInternal));

            memset(&mInternal, 0, sizeof(mInternal));

            auto lInfo = reinterpret_cast<BITMAPINFO*>(mInternal);

            lInfo->bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
            lInfo->bmiHeader.biPlanes   =  1;
            lInfo->bmiHeader.biBitCount = 24;
        }

        // ===== Window =====================================================

        Window_Bitmap::~Window_Bitmap() {}

        void Window_Bitmap::Show()
        {
            assert(nullptr != mInternal);
            assert(0 < mZoom);

            auto lSizeX_px = mBitmap.GetSizeX_px();
            auto lSizeY_px = mBitmap.GetSizeY_px();

            mSizeX_px = lSizeX_px * mZoom;
            mSizeY_px = lSizeY_px * mZoom;

            ValidateConfig();

            assert(0 < mSizeX_px);
            assert(0 < mSizeY_px);

            Create();

            auto lInfo = reinterpret_cast<BITMAPINFO*>(mInternal);

            lInfo->bmiHeader.biWidth  = lSizeX_px;
            lInfo->bmiHeader.biHeight = lSizeY_px;

            if (PERIOD_DISABLE != mPeriod_ms)
            {
                auto lTimer = SetTimer(GetWindow(), 1, mPeriod_ms, nullptr);
                assert(0 != lTimer);
            }

            Run();
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Window =====================================================

        LRESULT Window_Bitmap::WindowProc(UINT aMsg, WPARAM aParamW, LPARAM aParamL)
        {
            LRESULT lResult;

            switch (aMsg)
            {
            case WM_TIMER: lResult = On_WM_TIMER(); break;

            default: lResult = Window::WindowProc(aMsg, aParamW, aParamL);
            }

            return lResult;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // ===== Window =====================================================

        void Window_Bitmap::Paint(HDC aDC)
        {
            assert(0 < mSizeX_px);
            assert(0 < mSizeY_px);

            auto lInfo = reinterpret_cast<const BITMAPINFO*>(mInternal);

            auto lRet = StretchDIBits(aDC,
                0, 0, mSizeX_px            , mSizeY_px,
                0, 0, mBitmap.GetSizeX_px(), mBitmap.GetSizeY_px(),
                mBitmap.GetData(), lInfo, DIB_RGB_COLORS, SRCCOPY);
        }

        void Window_Bitmap::ValidateConfig()
        {
            KMS_EXCEPTION_ASSERT(0 < mZoom, WGDI_CONFIG_ERROR, "Invalid zoom", "");

            Window::ValidateConfig();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        LRESULT Window_Bitmap::On_WM_TIMER()
        {
            auto lRet = InvalidateRect(GetWindow(), nullptr, FALSE);
            assert(lRet);

            return 0;
        }

    }
}
