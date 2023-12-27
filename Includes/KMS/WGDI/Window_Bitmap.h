
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WGDI/Window_Bitmap.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Graph/Bitmap.h>
#include <KMS/WGDI/Window.h>

namespace KMS
{
    namespace WGDI
    {

        class Window_Bitmap final : public Window
        {

        public:

            static const uint32_t PERIOD_DEFAULT_ms;
            static const uint32_t PERIOD_MAX_ms;
            static const uint8_t  ZOOM_DEFAULT;
            static const uint8_t  ZOOM_MAX;
            static const uint8_t  ZOOM_MIN;

            uint32_t mPeriod_ms;
            uint8_t  mZoom;

            Window_Bitmap();

            void SetBitmap(Graph::Bitmap* aBitmap);

            // ===== Window =================================================
            virtual ~Window_Bitmap();

        // Internal

            // ===== Window =================================================
            virtual LRESULT WindowProc(UINT aMsg, WPARAM aParamW, LPARAM aParamL);

        protected:

            Graph::Bitmap* GetBitmap();

            // ===== Window =================================================
            virtual unsigned int OnStarting(void* aSender, void* aData);
            virtual void Paint(HDC aDC);
            virtual void ValidateConfig();

        private:

            LRESULT On_WM_TIMER();

            Graph::Bitmap* mBitmap;

            uint8_t mInternal[48];

        };

    }
}
