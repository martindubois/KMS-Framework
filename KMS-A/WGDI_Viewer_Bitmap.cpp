
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WGDI_BitmapViewer.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Main.h>

#include <KMS/WGDI/Viewer_Bitmap.h>

// Configurations
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_FILE_NAME("FileName = {Path}");
static const KMS::Cfg::MetaData MD_PERIOD_ms("Period_ms = {Period}");
static const KMS::Cfg::MetaData MD_TITLE    ("Title = {Tilte}");
static const KMS::Cfg::MetaData MD_ZOOM     ("Zoom = {Zoom}");

namespace KMS
{
    namespace WGDI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Viewer_Bitmap::FILE_NAME_DEFAULT = "";

        int Viewer_Bitmap::Main(int aCount, const char** aVector)
        {
            KMS_MAIN_BEGIN;
            {
                Viewer_Bitmap lVB;

                lConfigurator.AddConfigurable(&lVB);

                KMS_MAIN_PARSE_ARGS(aCount, aVector);

                KMS_MAIN_VALIDATE;

                lResult = lVB.Run();
            }
            KMS_MAIN_END;

            KMS_MAIN_RETURN;
        }

        Viewer_Bitmap::Viewer_Bitmap()
            : mFileName(FILE_NAME_DEFAULT)
            , mPeriod_ms(&mWindow.mPeriod_ms)
            , mTitle    (&mWindow.mTitle)
            , mZoom     (&mWindow.mZoom)
        {
            AddEntry("FileName" , &mFileName , false, &MD_FILE_NAME);
            AddEntry("Period_ms", &mPeriod_ms, false, &MD_PERIOD_ms);
            AddEntry("Title"    , &mTitle    , false, &MD_TITLE    );
            AddEntry("Zoom"     , &mZoom     , false, &MD_ZOOM     );
        }

        int Viewer_Bitmap::Run()
        {
            FileFormat::BMP lFile(File::Folder::NONE, mFileName);

            lFile.Open();

            mWindow.mBitmap.Init(&lFile);

            mWindow.Show();

            return 0;
        }

        // ===== DI::Container ==============================================

        Viewer_Bitmap::~Viewer_Bitmap() {}

        void Viewer_Bitmap::Validate() const
        {
            DI::Dictionary::Validate();

            KMS_EXCEPTION_ASSERT(File::Folder::NONE.DoesFileExist(mFileName.Get()), RESULT_INVALID_CONFIG, "The bitmap file does not exist", mFileName.Get());
            KMS_EXCEPTION_ASSERT(!mTitle.GetString().empty(), RESULT_INVALID_CONFIG, "Empty window title", "");
            KMS_EXCEPTION_ASSERT(Window_Bitmap::ZOOM_MAX >= mZoom, RESULT_INVALID_CONFIG, "The zoom is not valid", mZoom);
            KMS_EXCEPTION_ASSERT(Window_Bitmap::ZOOM_MIN <= mZoom, RESULT_INVALID_CONFIG, "The zoom is not valid", mZoom);
        }

    }
}
