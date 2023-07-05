
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Depend.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/String.h>
#include <KMS/Text/File_ASCII.h>

#include <KMS/Build/Depend.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void InsertStringSet(KMS::StringSet_ASCII* aInOut, const KMS::StringSet_ASCII* aIn);

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Depend::Depend(const DI::Array& aIncludes, const File::Folder& aFolder) : mFileCount(0), mFolder(aFolder), mIncludes(aIncludes)
        {
        }

        Depend::~Depend()
        {
            for (const auto& lPair : mCache)
            {
                assert(NULL != lPair.second);

                delete lPair.second;
            }
        }

        // ASSUMPTION All important #include statements start at the
        //            beginning of the line.

        // ASSUMPTION All component folder are directly in the product
        //            folder.

        StringSet_ASCII* Depend::ParseFile(const char* aFile)
        {
            assert(NULL != aFile);

            mFileCount++;

            StringSet_ASCII* lResult;

            auto lIt = mCache.find(aFile);
            if (mCache.end() != lIt)
            {
                assert(NULL != lIt->second);

                lResult = lIt->second;
            }
            else
            {
                lResult = new StringSet_ASCII;

                // Important to add it to the cache before filling the set to
                // avoid infinit recursivity.
                mCache.insert(FileMap::value_type(aFile, lResult));

                Text::File_ASCII lFile;

                lFile.Read(mFolder, aFile);

                for (const auto& lLine : lFile.mLines)
                {
                    char lInclude[FILE_LENGTH];

                    if (1 == sscanf_s(lLine.c_str(), "#include <%[^>]>", lInclude SizeInfo(lInclude)))
                    {
                        LocateDependency(lResult, lInclude);
                    }
                    else if (1 == sscanf_s(lLine.c_str(), "#include \"%[^\"]\"", lInclude SizeInfo(lInclude)))
                    {
                        if (mFolder.DoesFileExist(lInclude))
                        {
                            AddDependency(lResult, lInclude);
                        }
                        else if (!LocateLocalDependency(lResult, lInclude, aFile))
                        {
                            LocateDependency(lResult, lInclude);
                        }
                    }
                }
            }

            return lResult;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Depend::Display(std::ostream& aOut) const
        {
            aOut << mFolder.GetPath() << "\n";
            aOut << "    " << mFileCount << " files\n";
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // Only add the dependency if it is not already in the list.
        void Depend::AddDependency(StringSet_ASCII* aInOut, const char* aFile)
        {
            assert(NULL != aInOut);
            assert(NULL != aFile);

            if (aInOut->end() == aInOut->find(aFile))
            {
                aInOut->insert(aFile);

                StringSet_ASCII* lRet = ParseFile(aFile);
                assert(NULL != lRet);

                InsertStringSet(aInOut, lRet);
            }
        }

        void Depend::LocateDependency(StringSet_ASCII* aInOut, const char* aFile)
        {
            assert(NULL != aFile);

            for (const auto& lEntry : mIncludes.mInternal)
            {
                auto lF_Include = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lF_Include);

                char lHeader[PATH_LENGTH];

                sprintf_s(lHeader SizeInfo(lHeader), "../%s/%s", lF_Include->Get(), aFile);

                if (mFolder.DoesFileExist(lHeader))
                {
                    AddDependency(aInOut, lHeader);
                    break;
                }
            }
        }

        bool Depend::LocateLocalDependency(StringSet_ASCII* aInOut, const char* aFile, const char* aFrom)
        {
            assert(NULL != aFile);
            assert(NULL != aFrom);

            bool lResult = false;

            char lFrom[PATH_LENGTH];

            strcpy_s(lFrom, aFrom);

            auto lPtr = strrchr(lFrom, '/');
            if (NULL != lPtr)
            {
                *lPtr = '\0';

                File::Folder lFolder(mFolder, lFrom);

                if (lFolder.DoesFileExist(aFile))
                {
                    char lHeader[PATH_LENGTH];

                    sprintf_s(lHeader SizeInfo(lHeader), "%s/%s", lFrom, aFile);

                    AddDependency(aInOut, lHeader);
                    lResult = true;
                }
            }

            return lResult;
        }

    }
}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const Build::Depend& aTM)
{
    aTM.Display(aOut);

    return aOut;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void InsertStringSet(StringSet_ASCII* aInOut, const StringSet_ASCII* aIn)
{
    assert(NULL != aInOut);
    assert(NULL != aIn);

    for (const auto& lStr : *aIn)
    {
        aInOut->insert(lStr);
    }
}
