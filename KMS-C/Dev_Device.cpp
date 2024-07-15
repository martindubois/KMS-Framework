
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Dev/Device.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_INDEX("Index = {Numeber}");
static const KMS::Cfg::MetaData MD_LINK ("Link = {Link}");

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const uint8_t Device::INDEX_DEFAULT = 0;
        const char*   Device::LINK_DEFAULT  = "";

        Device::Device()
            : mConnectFlags(IDevice::FLAG_ACCESS_READ | IDevice::FLAG_ACCESS_WRITE)
            , mIndex(INDEX_DEFAULT)
            , mLink (LINK_DEFAULT)
            , mHandle(INVALID_HANDLE_VALUE)
        {
            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mIndex, false); AddEntry("Index", lEntry, &MD_INDEX);
            lEntry.Set(&mLink , false); AddEntry("Link" , lEntry, &MD_LINK);

            Construct_OSDep();
        }

        Device::~Device()
        {
            if (IsConnected())
            {
                Disconnect();
            }
        }

        // ===== Stream::IStream ============================================

        bool Device::IsConnected() const { return INVALID_HANDLE_VALUE != mHandle; }

        // ===== IDevice ====================================================

        void Device::SetConnectFlags(unsigned int aFlags) { mConnectFlags = aFlags; }

        void Device::ClearReadBuffer() { assert(false); }

        // ===== DI::Container ==============================================

        void Device::Validate() const
        {
            DI::Dictionary::Validate();

            // TODO
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Device::LinkFromIndex()
        {
            KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid device configuration", "");
        }

    }
}
