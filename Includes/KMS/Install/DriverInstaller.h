
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Install/DriverInstaller.h
// Library   KMS-C

#pragma once

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/DI/Boolean.h>
#include <KMS/DI/GUID.h>
#include <KMS/DI/Dictionary.h>

namespace KMS
{
    namespace Install
    {

        /// \brief TODO Remove use of DI namespace
        class DriverInstaller final : public DI::Dictionary
        {

        public:

            static const bool UNINSTALL_DRIVER_DEFAULT;

            DI::Boolean mUninstallDriver;

            #ifdef _KMS_WINDOWS_
                static const bool   DELETE_DEVICE_CLASS_DEFAULT;
                static const ::GUID DEVICE_CLASS_DEFAULT;

                DI::Boolean mDeleteDeviceClass;
                DI::GUID    mDeviceClass;
            #endif

            DriverInstaller();

            void Run();

            void UninstallDriver();

            #ifdef _KMS_WINDOWS_
                void DeleteDeviceClass();
            #endif

            // ===== DI::Dictionary =========================================
            virtual void Validate();

        private:

            void Construct_OSDep();

            void Run_OSDep();

        };

    }
}
