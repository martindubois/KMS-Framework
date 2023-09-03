
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/FileFormat_ELF.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/FileFormat/ELF.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MAGIC (0x464c457f)

#define CLASS_32_BIT (1)
#define CLASS_64_BIT (2)

namespace KMS
{
    namespace FileFormat
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        ELF::ELF(const File::Folder& aFolder, const char* aFile) : mBinary(aFolder, aFile)
        {
            mHeader = reinterpret_cast<Header32*>(mBinary.Map(sizeof(Header32)));

            auto lMappedSize_byte = mBinary.GetMappedSize();

            KMS_EXCEPTION_ASSERT(MAGIC                == mHeader->mMagic                   , RESULT_FILE_FORMAT_ERROR, "Invalid magic"              , mHeader->mMagic);
            KMS_EXCEPTION_ASSERT(CLASS_32_BIT         == mHeader->mClass                   , RESULT_FILE_FORMAT_ERROR, "Not a 32 bit file"          , mHeader->mClass);
            KMS_EXCEPTION_ASSERT(sizeof(Header32)     <= mHeader->mHeaderSize_byte         , RESULT_FILE_FORMAT_ERROR, "Invalid header size"        , mHeader->mHeaderSize_byte);
            KMS_EXCEPTION_ASSERT(sizeof(ProgHeader32) == mHeader->mProgHeaderEntrySize_byte, RESULT_FILE_FORMAT_ERROR, "Invalid program header size", mHeader->mProgHeaderEntrySize_byte);
            KMS_EXCEPTION_ASSERT(sizeof(SectHeader32) == mHeader->mSectHeaderEntrySize_byte, RESULT_FILE_FORMAT_ERROR, "Invalid section header size", mHeader->mSectHeaderEntrySize_byte);

            uint32_t lSize_byte;

            lSize_byte = mHeader->mProgHeaderOffset_byte + sizeof(ProgHeader32) * mHeader->mProgHeaderNumber;
            KMS_EXCEPTION_ASSERT(lMappedSize_byte >= lSize_byte, RESULT_FILE_FORMAT_ERROR, "Corrupeted file", lSize_byte);

            lSize_byte = mHeader->mSectHeaderOffset_byte + sizeof(SectHeader32) * mHeader->mSectHeaderNumber;
            KMS_EXCEPTION_ASSERT(lMappedSize_byte >= lSize_byte, RESULT_FILE_FORMAT_ERROR, "Corrupeted file", lSize_byte);

            auto lBase = reinterpret_cast<const uint8_t*>(mHeader);

            mProgHeaders = reinterpret_cast<const ProgHeader32*>(lBase + mHeader->mProgHeaderOffset_byte);
            mSectHeaders = reinterpret_cast<const SectHeader32*>(lBase + mHeader->mSectHeaderOffset_byte);
        }

        ELF::~ELF()
        {
        }

        unsigned int ELF::CreateFlashImage(uint32_t aStart, uint32_t aSize_byte, void* aOut) const
        {
            memset(aOut, 0, aSize_byte);

            auto lBase = reinterpret_cast<const uint8_t*>(mHeader);
            auto lOut  = reinterpret_cast<      uint8_t*>(aOut);

            unsigned int lResult_byte = 0;

            auto lEnd = aStart + aSize_byte;

            for (unsigned int i = 0; i < mHeader->mProgHeaderNumber; i++)
            {
                auto lPH = mProgHeaders + i;

                switch (lPH->mFlags)
                {
                case 0x5:
                case 0x7:
                    if (aStart <= lPH->mVirtualAddress)
                    {
                        if (lEnd > lPH->mVirtualAddress)
                        {
                            auto lOutOffset_byte = lPH->mVirtualAddress - aStart;

                            KMS_EXCEPTION_ASSERT(aSize_byte > lOutOffset_byte + lPH->mMemSize_byte, RESULT_FILE_FORMAT_ERROR, "Invalid memory segment", lPH->mVirtualAddress);

                            memcpy(lOut + lOutOffset_byte, lBase + lPH->mFileOffset_byte, lPH->mFileSize_byte);

                            lResult_byte += lPH->mFileSize_byte;
                        }
                    }
                    else
                    {
                        KMS_EXCEPTION_ASSERT(aStart >= lPH->mVirtualAddress + lPH->mMemSize_byte, RESULT_FILE_FORMAT_ERROR, "Invalid memory segment", lPH->mVirtualAddress);
                    }
                    break;
                }
            }

            return lResult_byte;
        }

        void ELF::DisplayHeaders(FILE* aOut) const
        {
            auto lOut = (nullptr == aOut) ? stdout : aOut;

            fprintf(lOut, "Header\n");
            mHeader->Display(lOut);

            unsigned int i;

            for (i = 0; i < mHeader->mProgHeaderNumber; i++)
            {
                fprintf(lOut, "  Program header %u\n", i);
                mProgHeaders[i].Display(lOut);
            }

            for (i = 0; i < mHeader->mSectHeaderNumber; i++)
            {
                fprintf(lOut, "  Section header %u\n", i);
                mSectHeaders[i].Display(lOut);
            }
        }

        void ELF::Header32::Display(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "  00 Magic                      : 0x%08x\n"
                "  04 Class                       : %u = %s\n"
                "  05 Endianness                  : %u = %s\n"
                "  06 Version byte                : %u\n"
                "  07 OS                          : 0x%02x = %s\n"
                "  08 Version ABI                 : %u\n"
                "  10 Type                        : 0x%04x = %s\n"
                "  12 Machine                     : 0x%04x = %s\n"
                "  14 Version                     : %u\n"
                "  18 Entry point                 : 0x%08x\n"
                "  1c Program header offset       : %u bytes\n"
                "  20 Section header offset       : %u bytes\n"
                "  24 Flags                       : 0x%08x\n"
                "  28 Header size                 : %u bytes\n"
                "  2a Program header entry size   : %u bytes\n"
                "  2c Program header number       : %u\n"
                "  2e Section header entry size   : %u bytes\n"
                "  30 Section header number       : %u\n"
                "  32 Section header string index : %u\n",
                mMagic,
                mClass, GetClass_Name(),
                mEndianness, GetEndianness_Name(),
                mVersionByte,
                mOS, GetOS_Name(),
                mVersionABI,
                mType, GetType_Name(),
                mMachine, GetMachine_Name(),
                mVersion,
                mEntryPoint,
                mProgHeaderOffset_byte,
                mSectHeaderOffset_byte,
                mFlags,
                mHeaderSize_byte,
                mProgHeaderEntrySize_byte,
                mProgHeaderNumber,
                mSectHeaderEntrySize_byte,
                mSectHeaderNumber,
                mSectHeaderStringIndex);
        }

        void ELF::ProgHeader32::Display(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "    00 Type             : 0x%08x = %s\n"
                "    04 File offset      : %u bytes\n"
                "    08 Virtual address  : 0x%08x\n"
                "    0c Physical address : 0x%08x\n"
                "    10 File size        : %u bytes\n"
                "    14 Memory size      : %u bytes\n"
                "    18 Flags            : 0x%08x\n"
                "    1c Alignment        : %u bytes\n",
                mType, GetType_Name(),
                mFileOffset_byte,
                mVirtualAddress,
                mPhysicalAddress,
                mFileSize_byte,
                mMemSize_byte,
                mFlags,
                mAlignment_byte);
        }

        void ELF::SectHeader32::Display(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "    00 Name index        : %u\n"
                "    04 Type              : 0x%08x = %s\n"
                "    08 Flags             : 0x%08x\n"
                "    0c Virtual address   : 0x%08x\n"
                "    10 File offset       : %u bytes\n"
                "    14 File size         : %u bytes\n"
                "    18 Link              : %u\n"
                "    1c Info              : %u\n"
                "    20 Address alignment : %u bytes\n"
                "    24 Entry size        : %u bytes\n",
                mNameIndex,
                mType, GetType_Name(),
                mFlags,
                mVirtualAddress,
                mFileOffset_byte,
                mFileSize_byte,
                mLink,
                mInfo,
                mAddressAlignment_byte,
                mEntrySize_byte);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        const char* ELF::Header32::GetClass_Name() const
        {
            auto lResult = "Invalid";

            switch (mClass)
            {
            case CLASS_32_BIT: lResult = "32-bit"; break;
            case CLASS_64_BIT: lResult = "64-bit"; break;

            default: assert(false);
            }

            return lResult;
        }

        const char* ELF::Header32::GetEndianness_Name() const
        {
            auto lResult = "Invalid";

            switch (mEndianness)
            {
            case 1: lResult = "Little endian"; break;
            case 2: lResult = "Big endian"   ; break;

            default: assert(false);
            }

            return lResult;
        }

        const char* ELF::Header32::GetMachine_Name() const
        {
            auto lResult = "Unknown";

            switch (mMachine)
            {
            case 0x5670: lResult = "MC56F8007"; break;
            }

            return lResult;
        }

        const char* ELF::Header32::GetOS_Name() const
        {
            auto lResult = "Unknown";

            switch (mOS)
            {
            case 0x00: lResult = "System V"; break;
            case 0x03: lResult = "Linux"   ; break;
            }

            return lResult;
        }

        const char* ELF::Header32::GetType_Name() const
        {
            auto lResult = "Invalid";

            switch (mType)
            {
            case 0x00: lResult = "Unknown"         ; break;
            case 0x01: lResult = "Relocatable file"; break;
            case 0x02: lResult = "Executable file" ; break;
            case 0x03: lResult = "Shared object"   ; break;
            case 0x04: lResult = "Core file"       ; break;

            default: assert(false);
            }

            return lResult;
        }

        const char* ELF::ProgHeader32::GetType_Name() const
        {
            auto lResult = "Invalid";

            switch (mType)
            {
            case 0x00: lResult = "Unused"                       ; break;
            case 0x01: lResult = "Loadable segment"             ; break;
            case 0x02: lResult = "Dynamic linking information"  ; break;
            case 0x03: lResult = "Interpreter information"      ; break;
            case 0x04: lResult = "Auxiliary information"        ; break;
            case 0x05: lResult = "Reserved"                     ; break;
            case 0x06: lResult = "Program header table"         ; break;
            case 0x07: lResult = "Thread-Local Storage template"; break;

            default: assert(false);
            }

            return lResult;
        }

        const char* ELF::SectHeader32::GetType_Name() const
        {
            auto lResult = "Invalid";

            switch (mType)
            {
            case 0x00: lResult = "Unused"                         ; break;
            case 0x01: lResult = "Program data"                   ; break;
            case 0x02: lResult = "Symbol table"                   ; break;
            case 0x03: lResult = "String table"                   ; break;
            case 0x04: lResult = "Ralocation entries with addends"; break;
            case 0x05: lResult = "Symbol hash table"              ; break;
            case 0x06: lResult = "Dynamic linking information"    ; break;
            case 0x07: lResult = "Notes"                          ; break;
            case 0x08: lResult = "Program space with no data"     ; break;
            case 0x09: lResult = "Relocation entries, no addends" ; break;
            case 0x0a: lResult = "Reserved"                       ; break;
            case 0x0b: lResult = "Dynamic linker symbol table"    ; break;
            case 0x0e: lResult = "Array of constructors"          ; break;
            case 0x0f: lResult = "Array of destructors"           ; break;
            case 0x10: lResult = "Array of pre-constructors"      ; break;
            case 0x11: lResult = "Section group"                  ; break;
            case 0x12: lResult = "Extended section indices"       ; break;

            default: assert(false);
            }

            return lResult;
        }

    }
}
