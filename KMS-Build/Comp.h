
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <string>

// ===== Includes ===========================================================
#include <KMS/Script/Script.h>

// ===== Local ==============================================================
enum class Phase;

class Comp
{

public:

    Comp(const char* aName);

    virtual void Verify(Phase aPhase, KMS::Script::Script* aScript);

    virtual void Package(KMS::File::Folder* aTmpFolder, KMS::Script::Script* aScript);

    virtual void Export();

private:

    KMS::String_ASCII mName;

};

class CompList
{

public:

    ~CompList();

    // aComp  This method deletes it
    void Add(Comp* aComp);

    void Verify(Phase aPhase, KMS::Script::Script* aScript);

    void Package(KMS::File::Folder* aTmpFolder, KMS::Script::Script* aScript);

    void Export();

public:

    typedef std::list<Comp*> InternalList;

    InternalList mComps;

};
