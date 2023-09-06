
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Dev/Device.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_INDEX (0)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_INDEX      ("Index = {Numeber}");
static const KMS::Cfg::MetaData MD_INTERFACE  ("Interface = {GUID}");
static const KMS::Cfg::MetaData MD_LINK       ("Link = {Link}");
static const KMS::Cfg::MetaData MD_HARDWARE_ID("HardwareId = {Id}");
static const KMS::Cfg::MetaData MD_LOCATION   ("Location = {Location}");

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Device::Device()
            : mIndex(DEFAULT_INDEX)
            #ifdef _KMS_WINDOWS_
                , mHandle(INVALID_HANDLE_VALUE)
            #endif
        {
            AddEntry("Index", &mIndex, false, &MD_INDEX);
            AddEntry("Link" , &mLink , false, &MD_LINK);

            #ifdef _KMS_WINDOWS_
                AddEntry("HardwareId", &mHardwareId, false, &MD_HARDWARE_ID);
                AddEntry("Interface" , &mInterface , false, &MD_INTERFACE);
                AddEntry("Location"  , &mLocation  , false, &MD_LOCATION);

                ResetInterface();
            #endif
        }

        Device::~Device()
        {
            if (IsConnected())
            {
                Disconnect();
            }
        }

        void Device::SetIndex(unsigned int aI) { mIndex = aI; }
        void Device::SetLink (const char * aL) { assert(nullptr != aL); mLink = aL; }

        // ===== DI::Container ==============================================

        void Device::Validate() const
        {
            DI::Dictionary::Validate();

            // TODO
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        unsigned int Device::GetIndex() const { return mIndex; }

        void Device::LinkFromIndex()
        {
            KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid device configuration", "");
        }

    }
}
