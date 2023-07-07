
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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
#include <KMS/DI/GUID.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>

namespace KMS
{
    namespace Dev
    {

        class Device : public DI::Dictionary, public IDevice
        {

        public:

            Device();

            virtual ~Device();

            bool IsConnected() const;

            void SetIndex(unsigned int aI);

            void SetLink(const char* aL);

            unsigned int Control(unsigned int aCode, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);

            #ifdef _KMS_WINDOWS_

                operator HANDLE ();

                const char* GetHardwareId() const;
                void        SetHardwareId(const char* aHI);

                const GUID* GetInterface() const;
                void        ResetInterface();
                void        SetInterface(const GUID& aI);

                const char* GetLocation() const;
                void        SetLocation(const char* aHI);

            #endif

            // ===== IDevice ================================================

            virtual bool Connect   (unsigned int aFlags);
            virtual void Disconnect();

            virtual unsigned int Read (void* aOut, unsigned int aOutSize_byte, unsigned int aFlags = 0);
            virtual bool         Write(const void* aIn, unsigned int aInSize_byte);

        // Internal

            // ===== Configurable attributes ================================
            DI::UInt<uint32_t> mIndex;
            DI::String         mLink;

        protected:

            unsigned int GetIndex() const;

            virtual void LinkFromIndex();

            #ifdef _KMS_WINDOWS_

                virtual void LinkFromHardwareIdAndIndex();
                virtual void LinkFromHardwareIdLocationAndIndex();
                virtual void LinkFromInterfaceAndIndex();

            #endif

        private:

            NO_COPY(Device);

            #ifdef _KMS_WINDOWS_

                static GUID ToGUID(const char* aG);

                HANDLE mHandle;
                bool   mInterface_Valid;

                // ===== Configurable attributes ============================
                DI::String mHardwareId;
                DI::GUID   mInterface;
                DI::String mLocation;

            #endif

        };

    }
}
