
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/String.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <set>
#include <string>

namespace KMS
{

    typedef std::string  String_ASCII;
    typedef std::wstring String_UTF16;

    typedef std::list<String_ASCII> StringList_ASCII;
    typedef std::list<String_UTF16> StringList_UTF16;
    
    typedef std::set<String_ASCII> StringSet_ASCII;
    typedef std::set<String_UTF16> StringSet_UTF16;

}
