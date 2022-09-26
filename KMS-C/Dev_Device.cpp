
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>

#include <KMS/Dev/Device.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_INDEX (0)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::DI::MetaData MD_INDEX      ("Index"    , "Index = {Numeber}");
static const KMS::DI::MetaData MD_INTERFACE  ("Interface", "Interface = {GUID}");
static const KMS::DI::MetaData MD_LINK       ("Link"     , "Link = {Link}");
static const KMS::DI::MetaData MD_HARDWARE_ID("Link"     , "HardwareId = {Id}");
static const KMS::DI::MetaData MD_LOCATION   ("Location" , "Location = {Location}");

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int Device::FLAG_READ_ACCESS  = 0x00000001;
        const unsigned int Device::FLAG_READ_ALL     = 0x00000002;
        const unsigned int Device::FLAG_WRITE_ACCESS = 0x00000004;

        Device::Device()
            : DI::Dictionary(NULL)
            , mIndex(DEFAULT_INDEX, &MD_INDEX)
            , mLink("", &MD_LINK)
            #ifdef _KMS_WINDOWS_
                , mHardwareId("", &MD_HARDWARE_ID)
                , mInterface(&MD_INTERFACE)
                , mLocation("", &MD_LOCATION)
                , mHandle(INVALID_HANDLE_VALUE)
            #endif
        {
            AddEntry(&mIndex);
            AddEntry(&mLink);

            #ifdef _KMS_WINDOWS_
                AddEntry(&mHardwareId);
                AddEntry(&mInterface);
                AddEntry(&mLocation);

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
        void Device::SetLink (const char * aL) { assert(NULL != aL); mLink = aL; }

        // Protected
        // //////////////////////////////////////////////////////////////////

        unsigned int Device::GetIndex() const { return mIndex; }

        void Device::LinkFromIndex()
        {
            KMS_EXCEPTION(CONFIG_VALUE, "Invalid device configuration");
        }

    }
}
