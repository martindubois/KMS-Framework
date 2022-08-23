
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Depend.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Text/TextFile.h>

#include <KMS/Build/Depend.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void InsertStringSet(KMS::StringSet* aInOut, const KMS::StringSet* aIn);

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Depend::Depend(const StringSet& aIncludes, const File::Folder& aFolder) : mFolder(aFolder), mIncludes(aIncludes)
        {
        }

        Depend::~Depend()
        {
            for (const FileMap::value_type& lPair : mCache)
            {
                assert(NULL != lPair.second);

                delete lPair.second;
            }
        }

        // ASSUMPTION All important #include statements start at the
        //            beginning of the line.

        // ASSUMPTION All component folder are directly in the product
        //            folder.

        // ASSUMPTION All #include using "" includes local files.

        StringSet* Depend::ParseFile(const char* aFile)
        {
            printf("%s( \"%s\" )\n", __FUNCTION__, aFile);

            assert(NULL != aFile);

            StringSet* lResult;

            FileMap::iterator lIt = mCache.find(aFile);
            if (mCache.end() != lIt)
            {
                assert(NULL != lIt->second);

                lResult = lIt->second;
            }
            else
            {
                lResult = new StringSet;

                // Important to add it to the cache before filling the set to
                // avoid infinit recursivity.
                mCache.insert(FileMap::value_type(aFile, lResult));

                Text::TextFile lFile;

                lFile.Read(mFolder, aFile);

                for (const std::string& lLine : lFile.mLines)
                {
                    char lInclude[PATH_LENGTH];

                    if (1 == sscanf(lLine.c_str(), "#include <%[^>]>", lInclude))
                    {
                        for (const std::string& lF_Include : mIncludes)
                        {
                            char lHeader[PATH_LENGTH];

                            sprintf(lHeader, "../%s/%s", lF_Include.c_str(), lInclude);

                            if (mFolder.DoesFileExist(lHeader))
                            {
                                AddDependency(lResult, lHeader);
                                break;
                            }
                        }
                    }
                    else if (1 == sscanf(lLine.c_str(), "#include \"%[^\"]\"", lInclude))
                    {
                        AddDependency(lResult, lInclude);
                    }
                }
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // Only add the dependency if it is not already in the list.
        void AddDependency(StringSet* aInOut, const char* aFile)
        {
            assert(NULL != aInOut);
            assert(NULL != aFile);

            if (aInOut->end() == aInOut->find(aFile))
            {
                aInOut->insert(lHeader);

                StringSet* lRet = ParseFile(lHeader);
                assert(NULL != lRet);

                InsertStringSet(aInOut, lRet);
            }
        }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void InsertStringSet(KMS::StringSet* aInOut, const KMS::StringSet* aIn)
{
    assert(NULL != aInOut);
    assert(NULL != aIn);

    for (const std::string& lStr : *aIn)
    {
        aInOut->insert(lStr);
    }
}
