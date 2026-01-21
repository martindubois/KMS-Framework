
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Comp.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

Comp::Comp(const char* aName) : mName(aName)
{
    assert(nullptr != aName);
}

void Comp::Verify(Phase, Script::Script*) {}

void Comp::Package(KMS::File::Folder*, Script::Script*) {}

void Comp::Export() {}

CompList::~CompList()
{
    for (auto lC : mComps)
    {
        assert(nullptr != lC);

        delete lC;
    }
}

void CompList::Add(Comp* aComp)
{
    assert(nullptr != aComp);
    
    mComps.push_back(aComp);
}

void CompList::Verify(Phase aPhase, Script::Script* aScript)
{
    for (auto lC : mComps)
    {
        assert(nullptr != lC);

        lC->Verify(aPhase, aScript);
    }
}

void CompList::Package(File::Folder* aTmp_Root, Script::Script* aScript)
{
    for (auto lC : mComps)
    {
        assert(nullptr != lC);

        lC->Package(aTmp_Root, aScript);
    }
}

void CompList::Export()
{
    for (auto lC : mComps)
    {
        assert(nullptr != lC);

        lC->Export();
    }
}
