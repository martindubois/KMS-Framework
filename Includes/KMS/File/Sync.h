
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/Sync.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Boolean.h>
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

            int Run();

            // ===== DI::Container ==========================================
            virtual void Validate() const;

            // ===== DI::Object =============================================
            virtual ~Sync();

            // ===== Configurable attributes ================================
            DI::Boolean mVerbose;

        private:

            NO_COPY(Sync);

            void Run_Bidirectional();
            void Run_Unidirectional();

            void Run_Bidirectional(const DI::Array& aGroup);
            void Run_Bidirectional(FileInfoList * aA, FileInfoList * aB);

            // ===== Configurable attributes ================================
            DI::Dictionary mBidirectional;
            DI::Array      mUnidirectional;

        };

    }
}
