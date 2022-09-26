
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/Sync.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <map>

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>

namespace KMS
{
    namespace File
    {

        class FileInfoList;

        class Sync : public DI::Dictionary
        {

        public:

            static int Main(int aCount, const char ** aVector);

            Sync();

            ~Sync();

            int Run();

        private:

            Sync(const Sync &);

            const Sync & operator = (const Sync &);

            void Run_Bidirectional();
            void Run_Unidirectional();

            void Run_Bidirectional(const DI::Array& aGroup);
            void Run_Bidirectional(FileInfoList * aA, FileInfoList * aB);

            void VerifyConfig() const;

            // ===== Configurable attributes ================================
            DI::Dictionary mBidirectional;
            DI::Array      mUnidirectional;

        };

    }
}
