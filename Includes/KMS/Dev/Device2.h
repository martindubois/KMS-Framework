
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-C

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2026 KMS
/// \file      Includes/KMS/Dev/Device2.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Configurable.h>
#include <KMS/Dev/IDevice.h>

namespace KMS
{
    namespace Dev
    {

        /// \brief Configuration for the Device2 class
        /// \note This is what, here at KMS, we call a configuration class.
        ///       - copy allowed
        ///       - final
        ///       - no virtual method
        ///       - public members
        ///       - uses `KMS_Config_METHODS` macro to declare standard
        ///         configurtion methodes
        class Device2_Config final
        {

        public:

            KMS_Config_METHODS(Device2_Config);

            /// \brief Is the mInteface value valid
            /// \retval false
            /// \retval true
            bool IsInterfaceValid() const;

            /// \brief Is the mLink value valid
            /// \retval false
            /// \retval true
            bool IsLinkValid() const;

            /// \brief Set the mLink value
            /// \param aLink The new value
            void SetLink(const char* aLink);

            char    mLink[PATH_LENGTH];
            uint8_t mIndex;

            uint8_t mReserved[7];

            #ifdef _KMS_LINUX_

                void LinkFromIndex();

            #endif

            #ifdef _KMS_WINDOWS_

                void LinkFromInterfaceAndIndex();

                GUID mInterface;

            #endif

        };

        /// \brief Manage a connection to a device
        class Device2 final : public Configurable<Device2_Config>, public IDevice
        {

        public:

            /// \brief Retrieve how many device match the configuration
            /// \param aConfig The configuration - The mIndex field is ignored
            /// \retval The avaialble device count
            static unsigned int GetCount(const Device2_Config& aConfig);

            /// \brief Constructor
            Device2();

            /// \brief Destructor
            virtual ~Device2();

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

        private:

            NO_COPY(Device2);

            unsigned int mConnectFlags;

            #ifdef _KMS_LINUX_

                int mHandle;

            #endif

            #ifdef _KMS_WINDOWS_

                HANDLE mHandle;

            #endif

        };

    }
}