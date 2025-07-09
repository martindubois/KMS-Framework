
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Executable.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Error.h"
#include "Phase.h"

#include "Tool_Executable.h"

using namespace KMS;

KMS_RESULT_STATIC(RESULT_EXECUTABLE_FAILED);

#define FILE_EXT_EXE ".exe"

// Static function declaration
// //////////////////////////////////////////////////////////////////////////

static const char* AddExtension(const char* aExecutable);

namespace Tool_Executable
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    Tool* CreateTool(ToolList* aTools, const Config&, File::Folder* aFolder, const char* aExecutable, uint32_t aTimeout_ms, Phase aVerify, Phase aExecute)
    {
        assert(nullptr != aTools);

        auto lResult = new Tool(aFolder, aExecutable, aTimeout_ms, aVerify, aExecute);

        aTools->Add(lResult);

        return lResult;
    }

    // Public
    // //////////////////////////////////////////////////////////////////////

    Tool::Tool(File::Folder* aFolder, const char* aExecutable, uint32_t aTimeout_ms, Phase aVerify, Phase aExecute)
        : ::Tool("Executable")
        , mExecute(aExecute)
        , mExecutable(aExecutable)
        , mFolder(aFolder)
        , mProcess(*aFolder, AddExtension(aExecutable))
        , mTimeout_ms(aTimeout_ms)
        , mVerify(aVerify)
    {
        assert(nullptr != aFolder);
        assert(0 < aTimeout_ms);
    }

    void Tool::AddArgument(const char* aArg) { mProcess.AddArgument(aArg); }

    // ===== Tool ===========================================================

    Tool::~Tool()
    {
        assert(nullptr != mFolder);

        delete mFolder;
    }

    void Tool::Execute(Phase aPhase)
    {
        std::cout << __FUNCTION__ << "( " << static_cast<unsigned int>(aPhase) << " ) - " << mExecutable.c_str() << std::endl;

        assert(0 < mTimeout_ms);

        if (mVerify == aPhase)
        {
            auto lExe = mExecutable.c_str();
            auto lExeExt = AddExtension(lExe);

            if (0 != strcmp("", mFolder->GetPath()))
            {
                if (!mFolder->DoesExist())
                {
                    Error_Folder_DoesNotExist(mFolder->GetPath());
                }

                if (!mFolder->DoesFileExist(lExeExt))
                {
                    Error_File_DoesNotExist(lExeExt);
                }
            }
        }

        if (mExecute == aPhase)
        {
            mProcess.Run(mTimeout_ms);

            auto lRet = mProcess.GetExitCode();

            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_EXECUTABLE_FAILED, "The compilation failed", mProcess.GetCmdLine());
        }
    }

}

// Static function declaration
// //////////////////////////////////////////////////////////////////////////

const char* AddExtension(const char* aExecutable)
{
    #ifdef _KMS_LINUX_
        return aExecutable;
    #endif

    #ifdef _KMS_WINDOWS_

        static char sExecutable[PATH_LENGTH];

        sprintf_s(sExecutable, "%s" FILE_EXT_EXE, aExecutable);

        return sExecutable;

    #endif
}
