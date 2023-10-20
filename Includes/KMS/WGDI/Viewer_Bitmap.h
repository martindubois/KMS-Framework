
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WGDI/Viewer_Bitmap.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/String_Expand.h>
#include <KMS/DI/UInt.h>
#include <KMS/WGDI/Window_Bitmap.h>

namespace KMS
{
    namespace WGDI
    {

        class Viewer_Bitmap final : public DI::Dictionary
        {

        public:

            static const char* FILE_NAME_DEFAULT;

            DI::String_Expand mFileName;

            DI::UInt_Ptr<uint32_t> mPeriod_ms;
            DI::String_Ptr         mTitle;
            DI::UInt_Ptr<uint8_t>  mZoom;

            static int Main(int aCount, const char** aVector);

            Viewer_Bitmap();

            int Run();

            // ===== DI::Container ==========================================
            ~Viewer_Bitmap();
            virtual void Validate() const;

        private:

            Window_Bitmap mWindow;

        };

    }
}

