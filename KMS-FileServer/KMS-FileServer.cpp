
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-FileServer/KMS-FileServer.cpp

// ===== Includes ===========================================================
#include <KMS/Banner.h>
#include <KMS/HTTP/FileServer.h>

// ===== KMS-ByteTool =======================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-FileServer");

    return KMS::HTTP::FileServer::Main(aCount, aVector);
}
