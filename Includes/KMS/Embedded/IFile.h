
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/IFile.h

#pragma once

namespace KMS
{
    namespace Embedded
    {

        class IFile
        {

        public:

            virtual unsigned int Read(void* aOut, unsigned int aOutSize_byte) = 0;

            virtual unsigned int Write(const void* aIn, unsigned int aInSize_byte) = 0;

        };

    }
}
