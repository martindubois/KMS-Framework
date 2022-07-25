
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

#include <KMS/Dev/Device.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_INDEX (0)

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
            : mIndex(DEFAULT_INDEX)
            #ifdef _KMS_WINDOWS_
                , mHandle(INVALID_HANDLE_VALUE)
            #endif
        {
            #ifdef _KMS_WINDOWS_
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

        // ===== Cfg::Configurable ==========================================

        void Device::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "===== KMS::Dev::Device =====\n"
                "Index\n"
                "    Set the index to the default value\n"
                "    Default: %u\n"
                "Index = {Value}\n"
                "    Device index\n"
                "Link\n"
                "    Clear the link\n"
                "Link = {Value}\n"
                "    Connection link\n",
                DEFAULT_INDEX);

            #ifdef _KMS_WINDOWS_
                fprintf(aOut,
                    "HardwareId\n"
                    "    Clear the hardware id\n"
                    "HardwareId = {Value}\n"
                    "    Hardware indentifiant\n"
                    "Interface\n"
                    "    Clear the interface"
                    "Interface = {GUID}\n"
                    "    Interface GUID\n"
                    "Location\n"
                    "    Clear the location\n"
                    "Location = {Value}\n"
                    "    Device location\n");
            #endif
        }

        bool Device::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Index") { SetIndex(DEFAULT_INDEX); return true; }
                CFG_IF("Link" ) { mLink.clear(); return true; }

                #ifdef _KMS_WINDOWS_
                    CFG_IF("HardwareId") { mHardwareId.clear(); return true; }
                    CFG_IF("Interface" ) { ResetInterface(); return true; }
                    CFG_IF("Location"  ) { mLocation.clear(); return true; }
                #endif
            }
            else
            {
                CFG_CONVERT("Index", SetIndex, Convert::ToUInt32);

                CFG_CALL("Link", SetLink);

                #ifdef _KMS_WINDOWS_
                    CFG_CALL("HardwareId", SetHardwareId);
                    CFG_CALL("Location"  , SetLocation  );

                    CFG_CONVERT("Interface", SetInterface, ToGUID);
                #endif
            }

            return Cfg::Configurable::SetAttribute(aA, aV);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        unsigned int Device::GetIndex() const { return mIndex; }

        void Device::LinkFromIndex()
        {
            KMS_EXCEPTION(CONFIG_VALUE, "Invalid device configuration");
        }

    }
}
