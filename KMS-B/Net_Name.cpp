
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Name.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Name.h>

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const sockaddr* Name::GetAddress() const
        {
            const sockaddr* lResult = nullptr;

            if (mFlags.mIPv4)
            {
                lResult = reinterpret_cast<const sockaddr*>(&mIPv4);
            }
            else if (mFlags.mIPv6)
            {
                lResult = reinterpret_cast<const sockaddr*>(&mIPv6);
            }

            return lResult;
        }

        unsigned int Name::GetAddressLength() const
        {
            unsigned int lResult_byte = 0;

            if (mFlags.mIPv4)
            {
                lResult_byte = sizeof(mIPv4);
            }
            else if (mFlags.mIPv6)
            {
                lResult_byte = sizeof(mIPv6);
            }

            return lResult_byte;
        }

        const sockaddr* Name::GetIPv4() const
        {
            const sockaddr* lResult = nullptr;

            if (mFlags.mIPv4)
            {
                lResult = reinterpret_cast<const sockaddr*>(&mIPv4);
            }

            return lResult;
        }

        const sockaddr* Name::GetIPv6() const
        {
            const sockaddr* lResult = nullptr;

            if (mFlags.mIPv6)
            {
                lResult = reinterpret_cast<const sockaddr*>(&mIPv6);
            }

            return lResult;
        }

        void Name::SetIPv4(const uint8_t* aIn)
        {
            memset(&mIPv4, 0, sizeof(mIPv4));

            // TODO

            mFlags.mIPv4 = true;
        }

        void Name::SetIPv4(const char* aIn)
        {
            unsigned int lAddr[4];

            auto lRet = sscanf_s(aIn, "%u.%u.%u.%u", lAddr + 0, lAddr + 1, lAddr + 2, lAddr + 3);
            KMS_EXCEPTION_ASSERT(4 == lRet, RESULT_INVALID_FORMAT, "Invalid IPv4 format", aIn);

            uint8_t lAddr8[4];

            for (unsigned int i = 0; i < 4; i++)
            {
                KMS_EXCEPTION_ASSERT(0xff >= lAddr[i], RESULT_INVALID_FORMAT, "Invalid IPv4 address", aIn);

                lAddr8[i] = lAddr[i];
            }

            SetIPv4(lAddr8);
        }

        void Name::SetName(const char* aIn)
        {
            memset(&mName, 0, sizeof(mName));

            strcpy_s(mName, aIn);

            mFlags.mName = true;
        }

        void Name::SetPortNumber(uint16_t aPortNumber)
        {
            mPortNumber = aPortNumber;

            mFlags.mPortNumber = true;
        }

        typedef struct
        {
            const char* mFormat;
            uint16_t    mPortNumber;
            Protocol    mProtocol;
        }
        URLFormat;

        static URLFormat URL_FORMAT_NPO[] =
        {
            { "ftp://%s:%u/%s"  , 0, Protocol::FTP   },
            { "http://%s:%u/%s" , 0, Protocol::HTTP  },
            { "https://%s:%u/%s", 0, Protocol::HTTPS },

            { nullptr, 0, Protocol::UNKNOWN }
        };

        static URLFormat URL_FORMAT_NO[] =
        {
            { "ftp://%s/%s"  ,  21, Protocol::FTP   },
            { "http://%s/%s" ,  80, Protocol::HTTP  },
            { "https://%s/%s", 843, Protocol::HTTPS },

            { nullptr, 0, Protocol::UNKNOWN }
        };

        static URLFormat URL_FORMAT_NP[] =
        {
            { "ftp://%s:%u/"  , 0, Protocol::FTP   },
            { "ftp://%s:%u"   , 0, Protocol::FTP   },
            { "http://%s:%u/" , 0, Protocol::HTTP  },
            { "http://%s:%u"  , 0, Protocol::HTTP  },
            { "https://%s:%u/", 0, Protocol::HTTPS },
            { "https://%s:%u" , 0, Protocol::HTTPS },

            { nullptr, 0, Protocol::UNKNOWN }
        };

        static URLFormat URL_FORMAT_N[] =
        {
            { "ftp://%s/"  ,  21, Protocol::FTP   },
            { "ftp://%s"   ,  21, Protocol::FTP   },
            { "http://%s/" ,  80, Protocol::HTTP  },
            { "http://%s"  ,  80, Protocol::HTTP  },
            { "https://%s/", 843, Protocol::HTTPS },
            { "https://%s" , 843, Protocol::HTTPS },

            { nullptr, 0, Protocol::UNKNOWN }
        };

        void Name::SetURL(const char* aIn, char* aOut, unsigned int aOutSize_byte)
        {
            unsigned int lPortNumber;

            unsigned int i;

            if (nullptr != aOut)
            {
                assert(0 < aOutSize_byte);

                i = 0;
                while (nullptr != URL_FORMAT_NPO[i].mFormat)
                {
                    if (3 == sscanf_s(URL_FORMAT_NPO[i].mFormat, mName, &lPortNumber, aOut SizeInfoV(aOutSize_byte)))
                    {
                        SetPortNumber(lPortNumber);
                        SetProtocol(URL_FORMAT_NPO[i].mProtocol);
                        mFlags.mName = true;
                        return;
                    }
                }

                i = 0;
                while (nullptr != URL_FORMAT_NO[i].mFormat)
                {
                    if (3 == sscanf_s(URL_FORMAT_NO[i].mFormat, mName, aOut SizeInfoV(aOutSize_byte)))
                    {
                        SetPortNumber(URL_FORMAT_NO[i].mPortNumber);
                        SetProtocol(URL_FORMAT_NO[i].mProtocol);
                        mFlags.mName = true;
                        return;
                    }
                }
            }

            i = 0;
            while (nullptr != URL_FORMAT_NP[i].mFormat)
            {
                if (3 == sscanf_s(URL_FORMAT_NP[i].mFormat, mName, &lPortNumber))
                {
                    SetPortNumber(lPortNumber);
                    SetProtocol(URL_FORMAT_NP[i].mProtocol);
                    mFlags.mName = true;
                    return;
                }

                i++;
            }

            i = 0;
            while (nullptr != URL_FORMAT_N[i].mFormat)
            {
                if (3 == sscanf_s(URL_FORMAT_N[i].mFormat, mName))
                {
                    SetPortNumber(URL_FORMAT_N[i].mPortNumber);
                    SetProtocol(URL_FORMAT_N[i].mProtocol);
                    mFlags.mName = true;
                    return;
                }

                i++;
            }
        }

        void Name::Resolve()
        {
            // TODO
        }

        // ===== Config =====================================================

        void Name::SetDefault()
        {
            mFlags.mIPv4       = false;
            mFlags.mIPv6       = false;
            mFlags.mName       = false;
            mFlags.mPortNumber = false;

            mPortNumber = 0;
            mProtocol   = Protocol::UNKNOWN;
        }

    }
}
