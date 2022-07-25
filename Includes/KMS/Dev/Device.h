
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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
#include <KMS/Cfg/Configurable.h>

namespace KMS
{
    namespace Dev
    {

        class Device : public Cfg::Configurable
        {

        public:

            static const unsigned int FLAG_READ_ACCESS;
            static const unsigned int FLAG_READ_ALL;
            static const unsigned int FLAG_WRITE_ACCESS;

            Device();

            virtual ~Device();

            bool IsConnected() const;

            void SetIndex(unsigned int aI);

            void SetLink(const char* aL);

            unsigned int Control(unsigned int aCode, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);

            // aFlags FLAG_READ_ACCESS, FLAG_WRITE_ACCESS
            virtual void Connect(unsigned int aFlags);

            virtual void Disconnect();

            // aFlags FLAG_READ_ALL
            unsigned int Read(void* aOut, unsigned int aOutSize_byte, unsigned int aFlags = 0);

            void Write(const void* aIn, unsigned int aInSize_byte);

            // ===== Cfg::Configurable ======================================
            virtual void DisplayHelp(FILE* aOut) const;
            virtual bool SetAttribute(const char* aN, const char* aV);

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

        protected:

            unsigned int GetIndex() const;

            virtual void LinkFromIndex();

            #ifdef _KMS_WINDOWS_

                virtual void LinkFromHardwareIdAndIndex();
                virtual void LinkFromHardwareIdLocationAndIndex();
                virtual void LinkFromInterfaceAndIndex();

            #endif

        private:

            Device(const Device&);

            const Device& operator = (const Device&);

            // ===== Configurable attributes ================================
            unsigned int mIndex;
            std::string  mLink;

            #ifdef _KMS_WINDOWS_

                static GUID ToGUID(const char* aG);

                HANDLE mHandle;

                // ===== Configurable attributes ============================
                std::string mHardwareId;
                GUID        mInterface;
                bool        mInterface_Valid;
                std::string mLocation;

            #endif

        };

    }
}
