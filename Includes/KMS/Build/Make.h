
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Make.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Build
    {

        class Make : public Config::Configurable
        {

        public:

            static int Main(int aCount, const char** aVector);

            Make();

            ~Make();

            void AddBinary (const char* aB);
            void AddLibrary(const char* aL);
            void AddTest   (const char* aT);

            void SetComponent    (const char* aC);
            void SetConfiguration(const char* aC);
            void SetOperation    (const char* aO);

            int Run();

            // ===== Config::Configurable ===================================
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual bool SetAttribute(const char *aA);
            virtual bool SetAttribute(const char* aA, const char* aV);

        private:

            enum class ComponentType
            {
                BINARY,
                LIBRARY,
                NONE,
                TEST,
            };

            Make(const Make&);

            const Make& operator = (const Make&);

            void Clean_Binaries();
            void Clean_Binary(const char*aB);
            void Clean_Component(const char* aC);
            void Clean_Libraries();
            void Clean_Library(const char* aL);
            void Clean_Tests();

            void Depend_Component(const char* aC);
            void Depend_Components(const StringSet& aComponents);

            void Make_Component(const char* aC);
            void Make_Components(const StringSet& aComponents);

            void Prepare();

            void Run_Clean();
            void Run_Depend();
            void Run_Make();

            void VerifyConfig();

            File::Folder mF_Bin_Cfg;
            File::Folder mF_Binaries;
            File::Folder mF_Product;
            File::Folder mF_Lib_Cfg;
            File::Folder mF_Libraries;

            // ===== Configurable attributes ================================
            StringSet     mBinaries;
            std::string   mComponent;
            ComponentType mComponentType;
            std::string   mConfiguration;
            StringSet     mLibraries;
            std::string   mOperation;
            StringSet     mTests;

        };

    }
}
