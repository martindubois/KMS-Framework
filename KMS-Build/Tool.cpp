
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Tool.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

ToolList::~ToolList()
{
    for (auto lT : mTools)
    {
        assert(nullptr != lT);

        delete lT;
    }
}

void ToolList::Add(Tool* aTool)
{
    assert(nullptr != aTool);

    mTools.push_back(aTool);
}

Tool::Tool(const char* aName) : mName(aName) { assert(nullptr != aName); }

void Tool::Execute(Phase, Script::Script*) {}

void ToolList::Execute(Phase aPhase, Script::Script* aScript)
{
    for (auto lT : mTools)
    {
        assert(nullptr != lT);

        lT->Execute(aPhase, aScript);
    }
}
