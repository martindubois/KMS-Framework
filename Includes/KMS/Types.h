
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Types.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <set>
#include <string>

namespace KMS
{

    enum class Radix
    {
        DECIMAL     = 10,
        HEXADECIMAL = 16,
    };

    // Internal

    typedef std::list<std::string>  StringList_ASCII;
    typedef std::list<std::wstring> StringList_UTF16;
    
    typedef std::set<std::string>  StringSet_ASCII;
    typedef std::set<std::wstring> StringSet_UTF16;

}
