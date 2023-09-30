
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Dev/Device.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_INDEX      ("Index = {Numeber}");
static const KMS::Cfg::MetaData MD_INTERFACE  ("Interface = {GUID}");
static const KMS::Cfg::MetaData MD_LINK       ("Link = {Link}");

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const uint8_t Device::INDEX_DEFAULT = 0;
        const char*   Device::LINK_DEFAULT  = "";

        Device::Device()
            : mIndex(INDEX_DEFAULT)
            , mLink (LINK_DEFAULT)
            #ifdef _KMS_WINDOWS_
                , mInterface(INTERFACE_DEFAULT)
                , mHandle(INVALID_HANDLE_VALUE)
            #endif
        {
            AddEntry("Index", &mIndex, false, &MD_INDEX);
            AddEntry("Link" , &mLink , false, &MD_LINK);

            #ifdef _KMS_WINDOWS_
                AddEntry("Interface" , &mInterface , false, &MD_INTERFACE);
            #endif
        }

        Device::~Device()
        {
            if (IsConnected())
            {
                Disconnect();
            }
        }

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
