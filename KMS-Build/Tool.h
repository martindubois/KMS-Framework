
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <string>

// ===== Includes ===========================================================
#include <KMS/Script/Script.h>

// ===== Local ==============================================================
enum class Phase;

class Tool
{

public:

    Tool(const char* aName);

    virtual void Execute(Phase aPhase, KMS::Script::Script* aScript);

private:

    KMS::String_ASCII mName;

};

class ToolList
{

public:

    ~ToolList();

    // aComp  This method deletes it
    void Add(Tool* aComp);

    void Execute(Phase aPhase, KMS::Script::Script* aScript);

public:

    typedef std::list<Tool*> InternalList;

    InternalList mTools;

};
