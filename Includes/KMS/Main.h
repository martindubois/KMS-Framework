
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Main.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Dbg/Log_Cfg.h>
#include <KMS/Dbg/Stats.h>
#include <KMS/Dbg/Stats_Timer.h>
#include <KMS/Install/Installer.h>

// Macros
// //////////////////////////////////////////////////////////////////////////

#define KMS_MAIN_BEGIN                                          \
    int lResult = __LINE__;                                     \
    auto lET = new KMS::Dbg::Stats_Timer("Main_ExecutionTime"); \
    lET->Start();                                               \
    try                                                         \
    {                                                           \
        KMS::Cfg::Configurator  lConfigurator;                  \
        KMS::Install::Installer lInstaller;                     \
        KMS::Dbg::Log_Cfg       lLogCfg(&KMS::Dbg::gLog);       \
        lConfigurator.AddConfigurable(&lInstaller);             \
        lConfigurator.AddConfigurable(&lLogCfg);                \
        lConfigurator.AddConfigurable(&Dbg::gStats)

#define KMS_MAIN_END          \
    }                         \
    KMS_CATCH_RESULT(lResult) \
    lET->Stop()

#define KMS_MAIN_PARSE_ARGS(C, V)                  \
    lConfigurator.ParseArguments((C) - 1, (V) + 1)

#define KMS_MAIN_RETURN return lResult

#define KMS_MAIN_VALIDATE     \
    lConfigurator.Validate(); \
    lInstaller.Run()
