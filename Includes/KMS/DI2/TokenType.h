
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI2/TokenType.h
// Library   KMS-A

#pragma once

namespace KMS
{
    namespace DI2
    {

        enum class TokenType
        {
            END       = 0x00000001,
            FLOAT     = 0x00000002,
            INT       = 0x00000004,
            NAME      = 0x00000008,
            OPERATOR  = 0x00000010,
            QUOTED    = 0x00000020,
            STRING    = 0x00000040,
            UINT      = 0x00000080,
            UINT_HEX  = 0x00000100,

            NONE = 0x00000000,
            ALL  = 0x000001ff,
        };

        inline TokenType operator | (TokenType aA, TokenType aB)
        {
            return static_cast<TokenType>(static_cast<unsigned int>(aA) | static_cast<unsigned int>(aB));
        }

        inline bool IsTokenTypePresent(TokenType aMask, TokenType aType)
        {
            auto lMask = static_cast<uint32_t>(aMask);
            auto lType = static_cast<uint32_t>(aType);

            return 0 != (lMask & lType);
        }

    }
}
