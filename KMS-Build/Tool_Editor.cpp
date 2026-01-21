
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Editor.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/Console/Color.h>
#include <KMS/DI/String.h>
#include <KMS/Text/File_ASCII.h>

// ==== Local ===============================================================
#include "Config.h"
#include "Phase.h"
#include "Tool.h"

#include "Tool_Editor.h"

using namespace KMS;

class T_Editor : public Tool
{

public:

    T_Editor(const char* aOperation, unsigned int aIndex, const KMS::Version& aVersion);

    // ===== Tool ===========================================================
    virtual void Execute(Phase aPhase, Script::Script* aScript) override;

private:

    void Execute_EDIT();

    unsigned int mIndex;
    String_ASCII mOperation;
    KMS::Version mVersion;

};

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void operator += (String_ASCII& aString, unsigned int aValue);

static void CreateTool(ToolList* aTools, const Config& aCfg, const char* aOperation, unsigned int aIndex);

static String_ASCII ProcessReplaceLine(const char* aIn, const KMS::Version& aVersion);

namespace Tool_Editor
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void CreateTools(ToolList* aTools, const Config& aCfg, const DI::Array& aEditOperations)
    {
        assert(nullptr != aTools);
        
        unsigned int lIndex = 0;

        for (const auto& lEOE : aEditOperations.mInternal)
        {
            auto lEO = dynamic_cast<const DI::String*>(lEOE.Get());
            assert(nullptr != lEO);

            CreateTool(aTools, aCfg, lEO->Get(), lIndex);

            lIndex++;
        }
    }

}

// Public
// //////////////////////////////////////////////////////////////////////////

T_Editor::T_Editor(const char* aOperation, unsigned int aIndex, const Version& aVersion)
    : Tool("Editor"), mIndex(aIndex), mOperation(aOperation), mVersion(aVersion)
{
    assert(nullptr != aOperation);
}

// ===== Tool ===============================================================

void T_Editor::Execute(Phase aPhase, Script::Script* aScript)
{
    switch (aPhase)
    {
    case Phase::EDIT:
        Execute_EDIT();
        break;
    }
}

// Private
// //////////////////////////////////////////////////////////////////////////

// SCRIPT  Script does not include the EDIT phase.
void T_Editor::Execute_EDIT()
{
    char lFile   [FILE_LENGTH];
    char lRegEx  [LINE_LENGTH];
    char lReplace[LINE_LENGTH];

    auto lRet = sscanf_s(mOperation.c_str(), "%[^;];%[^;];%[^\n\r]", lFile SizeInfo(lFile), lRegEx SizeInfo(lRegEx), lReplace SizeInfo(lReplace));
    KMS_EXCEPTION_ASSERT(3 == lRet, RESULT_INVALID_CONFIG, "Invalid edit operation", mOperation.c_str());

    auto lReplaceStr = ProcessReplaceLine(lReplace, mVersion);

    Text::File_ASCII lText;

    lText.Read(KMS::File::Folder::CURRENT, lFile);

    auto lCount = lText.ReplaceLines(lRegEx, lReplaceStr.c_str());
    if (0 == lCount)
    {
        std::cout << Console::Color::RED;
        {
            std::cout << "Edit operation " << mIndex << " - 0 line replaced";
        }
        std::cout << Console::Color::WHITE;
    }
    else
    {
        std::cout << "Edit operation " << mIndex << " - " << lCount << " line replaced";

        File::Folder::CURRENT.Backup(lFile);

        lText.Write(KMS::File::Folder::CURRENT, lFile);
    }

    std::cout << std::endl;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void operator += (String_ASCII& aString, unsigned int aValue)
{
    char lValue[32];

    sprintf_s(lValue, "%u", aValue);

    aString += lValue;
}

void CreateTool(ToolList* aTools, const Config& aCfg, const char* aOperation, unsigned int aIndex)
{
    assert(nullptr != aTools);

    auto lComp = new T_Editor(aOperation, aIndex, aCfg.GetVersion());

    aTools->Add(lComp);
}

String_ASCII ProcessReplaceLine(const char * aIn, const Version & aVersion)
{
    // --> INIT <--+
    //      |      |
    //      +--> PROCESS
    static const unsigned int STATE_INIT    = 0;
    static const unsigned int STATE_PROCESS = 1;

    assert(nullptr != aIn);

    auto         lIn = aIn;
    String_ASCII lResult;
    unsigned int lState = STATE_INIT;
    auto         lType = aVersion.GetType();

    if (0 >= strlen(lType))
    {
        lType = nullptr;
    }

    for (;;)
    {
        switch (lState)
        {
        case STATE_INIT:
            switch (*lIn)
            {
            case '\0': return lResult;
            case '{': lState = STATE_PROCESS; break;
            default: lResult += *lIn;
            }
            break;

        case STATE_PROCESS:
            switch (*lIn)
            {
            case '\0': KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid edition operation", aIn);
            case '}': lState = STATE_INIT; break;
            case 'M': lResult += aVersion.GetMajor(); break;
            case 'm': lResult += aVersion.GetMinor(); break;
            case 'B': lResult += aVersion.GetBuild(); break;
            case 'C': lResult += aVersion.GetCompat(); break;

            case 'T':
                if (nullptr != lType)
                {
                    lResult += "-";
                    lResult += aVersion.GetType();
                }
                break;

            default: lResult += *lIn;
            }
            break;

        default: assert(false);
        }

        lIn++;
    }
}
