
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WGDI_BitmapViewer.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Dbg/Log_Cfg.h>
#include <KMS/Dbg/Stats.h>
#include <KMS/Dbg/Stats_Timer.h>
#include <KMS/Installer.h>

#include <KMS/WGDI/Viewer_Bitmap.h>

// Configurations
// //////////////////////////////////////////////////////////////////////////

#define ZOOM_MAX (8)
#define ZOOM_MIN (1)

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

        int Viewer_Bitmap::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(nullptr != aVector);
            assert(nullptr != aVector[0]);

            int lResult = __LINE__;

            auto lET = new Dbg::Stats_Timer("Main_ExecutionTime");
            lET->Start();

            try
            {
                Cfg::Configurator lC;
                Installer         lInstaller;
                Dbg::Log_Cfg      lLogCfg(&Dbg::gLog);
                Viewer_Bitmap     lVB;

                lC.AddConfigurable(&lVB);

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lLogCfg);
                lC.AddConfigurable(&Dbg::gStats);

                lC.ParseArguments(aCount - 1, aVector + 1);

                lC.Validate();

                lInstaller.Run();

                lResult = lVB.Run();
            }
            KMS_CATCH_RESULT(lResult);

            lET->Stop();

            return lResult;
        }

        Viewer_Bitmap::Viewer_Bitmap()
            // ===== Configurable attribute =================================
            : mPeriod_ms(&mWindow.mPeriod_ms)
            , mTitle    (&mWindow.mTitle)
            , mZoom     (&mWindow.mZoom)
        {
            AddEntry("FileName" , &mFileName , false, &MD_FILE_NAME);
            AddEntry("Period_ms", &mPeriod_ms, false, &MD_PERIOD_ms);
            AddEntry("Title"    , &mTitle    , false, &MD_TITLE    );
            AddEntry("Zoom"     , &mZoom     , false, &MD_ZOOM     );
        }

        Viewer_Bitmap::~Viewer_Bitmap() {}

        int Viewer_Bitmap::Run()
        {
            FileFormat::BMP lFile(File::Folder::NONE, mFileName);

            lFile.Open();

            mWindow.mBitmap.Init(&lFile);

            mWindow.Show();

            return 0;
        }

        // ===== DI::Container ==============================================

        void Viewer_Bitmap::Validate() const
        {
            DI::Dictionary::Validate();

            KMS_EXCEPTION_ASSERT(File::Folder::NONE.DoesFileExist(mFileName.Get()), RESULT_INVALID_CONFIG, "The bitmap file does not exist", mFileName.Get());
            KMS_EXCEPTION_ASSERT(!mTitle.GetString().empty(), RESULT_INVALID_CONFIG, "Empty window title", "");
            KMS_EXCEPTION_ASSERT(ZOOM_MAX >= mZoom, RESULT_INVALID_CONFIG, "The zoom is not valid", mZoom);
            KMS_EXCEPTION_ASSERT(ZOOM_MIN <= mZoom, RESULT_INVALID_CONFIG, "The zoom is not valid", mZoom);
        }

    }
}
