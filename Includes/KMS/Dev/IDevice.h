
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2023-2026 KMS
/// \file      Includes/KMS/Dev/IDevice.h

#pragma once

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Stream/IStream.h>

namespace KMS
{
    namespace Dev
    {

        /// \brief Device interface
        class IDevice : public Stream::IStream
        {

        public:

            static const unsigned int FLAG_ACCESS_READ  = 0x00000001;
            static const unsigned int FLAG_ACCESS_WRITE = 0x00000002;
            static const unsigned int FLAG_OVERLAPPED   = 0x00000004;

            /// \brief Set connect flags
            /// \param aFlags See FLAG_ACCESS_READ, FLAG_ACCESS_WRITE, FLAG_OVERLAPPED
            virtual void SetConnectFlags(unsigned int aFlags) = 0;

            /// \brief Clear read buffer
            virtual void ClearReadBuffer() = 0;

            /// \brief Execute IOCtl
            /// \param aCode         The IOCtl code
            /// \param aIn           The input data
            /// \param aInSize_byte  The input data size
            /// \param aOut          The output data
            /// \param aOutSize_byte The output data size
            /// \retval The actual output data size
            virtual unsigned int Control(unsigned int aCode, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte) = 0;

            #ifdef _KMS_LINUX_

                virtual operator int () = 0;

            #endif

            #ifdef _KMS_WINDOWS_

                virtual operator HANDLE () = 0;

            #endif

        };

    }
}
