
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/FileFormat/ELF.h

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/File/Binary.h>
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace FileFormat
    {

        class ELF
        {

        public:

            ELF(const File::Folder& aFolder, const char* aFile);

            ~ELF();

            unsigned int CreateFlashImage(uint32_t aStart, uint32_t aSize_byte, void* aOut) const;

            void DisplayHeaders(FILE* aOut) const;

        private:

            class Header32
            {

            public:

                void Display(FILE* aOut) const;

                uint32_t mMagic;
                uint8_t  mClass;
                uint8_t  mEndianness;
                uint8_t  mVersionByte;
                uint8_t  mOS;
                uint8_t  mVersionABI;
                uint8_t  mReserved0[7];
                uint16_t mType;
                uint16_t mMachine;
                uint32_t mVersion;
                uint32_t mEntryPoint;
                uint32_t mProgHeaderOffset_byte;
                uint32_t mSectHeaderOffset_byte;
                uint32_t mFlags;
                uint16_t mHeaderSize_byte;
                uint16_t mProgHeaderEntrySize_byte;
                uint16_t mProgHeaderNumber;
                uint16_t mSectHeaderEntrySize_byte;
                uint16_t mSectHeaderNumber;
                uint16_t mSectHeaderStringIndex;

            private:

                const char* GetClass_Name     () const;
                const char* GetEndianness_Name() const;
                const char* GetOS_Name        () const;
                const char* GetMachine_Name   () const;
                const char* GetType_Name      () const;

            };

            class ProgHeader32
            {

            public:

                void Display(FILE* aOut) const;

                uint32_t mType;
                uint32_t mFileOffset_byte;
                uint32_t mVirtualAddress;
                uint32_t mPhysicalAddress;
                uint32_t mFileSize_byte;
                uint32_t mMemSize_byte;
                uint32_t mFlags;
                uint32_t mAlignment_byte;

            private:

                const char* GetType_Name() const;
                
            };

            class SectHeader32
            {

            public:

                void Display(FILE* aOut) const;

                uint32_t mNameIndex;
                uint32_t mType;
                uint32_t mFlags;
                uint32_t mVirtualAddress;
                uint32_t mFileOffset_byte;
                uint32_t mFileSize_byte;
                uint32_t mLink;
                uint32_t mInfo;
                uint32_t mAddressAlignment_byte;
                uint32_t mEntrySize_byte;

            private:

                const char* GetType_Name() const;

            };

            File::Binary        mBinary;
            const Header32    * mHeader;
            const ProgHeader32* mProgHeaders;
            const SectHeader32* mSectHeaders;

        };

    }
}
