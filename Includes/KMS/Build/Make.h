
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Make.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurable.h>
#include <KMS/File/Folder.h>
#include <KMS/Text/File_ASCII.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Build
    {

        class Depend;

        class Make : public Cfg::Configurable
        {

        public:

            static int Main(int aCount, const char** aVector);

            Make();

            ~Make();

            void AddBinary   (const char* aB);
            void AddInclude  (const char* aI);
            void AddLibrary  (const char* aL);
            void AddOperation(const char* aO);
            void AddTest     (const char* aT);

            void ResetBinaries  ();
            void ResetComponent ();
            void ResetIncludes  ();
            void ResetLibraries ();
            void ResetOperations();
            void ResetTests     ();

            void SetComponent    (const char* aC);
            void SetConfiguration(const char* aC);

            int Run();

            // ===== Cfg::Configurable ======================================
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual void DisplayHelp(FILE* aOut) const;
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
            void Depend_Components(const StringSet_ASCII& aComponents);
            void Depend_ParseMakeFile(Text::File_ASCII* aMF, StringSet_ASCII* aSources);
            void Depend_ParseSource(Depend* aDepend, const char* aSource, Text::File_ASCII* aMakeFile);

            void Make_Component(const char* aC);
            void Make_Components(const StringSet_ASCII& aComponents);

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
            StringSet_ASCII mBinaries;
            std::string     mComponent;
            ComponentType   mComponentType;
            std::string     mConfiguration;
            StringSet_ASCII mIncludes;
            StringSet_ASCII mLibraries;
            StringSet_ASCII mOperations;
            StringSet_ASCII mTests;

        };

    }
}
