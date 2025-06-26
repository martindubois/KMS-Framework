
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
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

void Comp::Verify(Phase) {}

void Comp::Package(KMS::File::Folder*) {}

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

void CompList::Verify(Phase aPhase)
{
    for (auto lC : mComps)
    {
        assert(nullptr != lC);

        lC->Verify(aPhase);
    }
}

void CompList::Package(File::Folder* aTmp_Root)
{
    for (auto lC : mComps)
    {
        assert(nullptr != lC);

        lC->Package(aTmp_Root);
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
