
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/SafeAPI.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Base.h>

// Macros
// //////////////////////////////////////////////////////////////////////////

#ifdef _KMS_WINDOWS_

    #define SizeInfo(V)  , static_cast<unsigned int>(sizeof(V))
    #define SizeInfoV(V) , V

#endif

#if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )

    #define SizeInfo(V)
    #define SizeInfoV(V)

#endif
