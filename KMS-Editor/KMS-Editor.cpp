
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Editor/KMS-Editor.cpp

#include <KMS/Base.h>

// ===== Includes ===========================================================
#include <KMS/Banner.h>
#include <KMS/Build/Editor.h>
#include <KMS/Build/Editor_DI2.h>
#include <KMS/DI2/Input.h>
#include <KMS/Main2.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

using namespace KMS;

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-Editor");

    KMS_MAIN2_BEGIN;

        Build::Editor_Config lConfig;

        DI2::Decode_ASCII_Arguments(&lConfig, &Build::Editor_Config_TYPE, &lArgList);

        KMS_MAIN2_CONFIG_END;

        Build::Editor lEditor;

        lEditor.Config_Set(lConfig);

        lEditor.Execute();

        lResult = 0;

    KMS_MAIN2_END;

    KMS_MAIN2_RETURN;
}
