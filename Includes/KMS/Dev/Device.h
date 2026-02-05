
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dev/Device.h

#pragma once

#include <KMS/Base.h>

// ===== C++ ================================================================
#include <string>

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Dev/IDevice.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/String_Expand.h>
#include <KMS/DI/UInt.h>

#ifdef _KMS_WINDOWS_
    #include <KMS/DI/GUID.h>
#endif

namespace KMS
{
    namespace Dev
    {

        /// \brief TODO Remove use of DI namespace
        class Device : public DI::Dictionary, public IDevice
        {

        public:

            static const uint8_t INDEX_DEFAULT;
            static const char  * LINK_DEFAULT;

            #ifdef _KMS_WINDOWS_
                static const ::GUID INTERFACE_DEFAULT;
            #endif

            DI::UInt<uint8_t> mIndex;
            DI::String_Expand mLink;

            #ifdef _KMS_WINDOWS_
                DI::GUID mInterface;
            #endif

        public:

            #ifdef _KMS_LINUX_
                static unsigned int GetCount(const char* aFormat);
            #endif

            #ifdef _KMS_WINDOWS_
                static unsigned int GetCount(const ::GUID* aInterface);
            #endif

            Device();

            virtual ~Device();

            // ===== DI::Container ==========================================
            virtual void Validate() const;

            // ===== IDevice ================================================
            virtual void         SetConnectFlags(unsigned int aFlags);
            virtual void         ClearReadBuffer();
            virtual unsigned int Control(unsigned int aCode, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);

            // ===== Stream::IStream ========================================
            virtual bool         IsConnected() const;
            virtual bool         Connect();
            virtual void         Disconnect();
            virtual unsigned int Read (void* aOut, unsigned int aOutSize_byte, unsigned int aFlags = 0);
            virtual bool         Write(const void* aIn, unsigned int aInSize_byte);

            #ifdef _KMS_LINUX_

                virtual operator int ();

            #endif

            #ifdef _KMS_WINDOWS_

                virtual operator HANDLE ();

            #endif

        protected:

            virtual void LinkFromIndex();

            #ifdef _KMS_WINDOWS_

                virtual void LinkFromInterfaceAndIndex();

            #endif

        private:

            NO_COPY(Device);

            void Construct_OSDep();

            unsigned int mConnectFlags;

            #ifdef _KMS_LINUX_

                int mHandle;

            #endif

            #ifdef _KMS_WINDOWS_

                static GUID ToGUID(const char* aG);

                HANDLE mHandle;

            #endif

        };

    }
}
