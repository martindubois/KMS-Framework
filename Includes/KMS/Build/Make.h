
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Make.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/CLI/Tool.h>
#include <KMS/DI/Array.h>
#include <KMS/DI/Enum.h>
#include <KMS/DI/String_Expand.h>
#include <KMS/File/Folder.h>
#include <KMS/Text/File_ASCII.h>

namespace KMS
{
    namespace Build
    {

        class Depend;

        class Make : public CLI::Tool
        {

        public:

            static const char* COMPONENT_DEFAULT;
            static const char* COMPONENT_TYPE_DEFAULT;
            static const char* CONFIGURATION_DEFAULT;
            static const char* MAKE_DEFAULT;
            static const char* PROCESSOR_DEFAULT;

            DI::Array         mCleanExtensions;
            DI::String        mComponent;
            DI::String        mConfiguration;
            DI::Array         mIncludes;
            DI::String_Expand mMake;
            DI::String        mProcessor;

            // ----- Build --------------------------------------------------
            DI::Array mBinaries;
            DI::Array mLibraries;
            DI::Array mTests;

            static const char* SILENCE[];

            static int Main(int aCount, const char** aVector);

            Make();

            ~Make();

            void AddBinary (const char* aB);
            void AddInclude(const char* aI);
            void AddLibrary(const char* aL);
            void AddTest   (const char* aT);

            // ===== CLI::Tool ==============================================
            virtual void DisplayHelp(FILE* aFile);
            virtual int  ExecuteCommand(const char* aC);
            virtual int  Run();

        private:

            enum class ComponentType
            {
                BINARY,
                LIBRARY,
                NONE,
                TEST,

                QTY
            };

            static const char* COMPONENT_TYPE_NAMES[];

            NO_COPY(Make);

            void Clean_Binaries();
            void Clean_Binary(const char*aB);
            void Clean_Component(const char* aC);
            void Clean_Libraries();
            void Clean_Library(const char* aL);
            void Clean_Tests();

            void Depend_Component(const char* aC);
            void Depend_Components(const DI::Array& aComponents);
            void Depend_ParseMakeFile(Text::File_ASCII* aMF, StringSet_ASCII* aSources);
            void Depend_ParseSource(Depend* aDepend, const char* aSource, Text::File_ASCII* aMakeFile);

            void Make_Component(const char* aC);
            void Make_Components(const DI::Array& aComponents);

            void Prepare();

            int Run_Clean();
            int Run_Depend();
            int Run_Make();

            void VerifyConfig();

            File::Folder mF_Bin_Cfg;
            File::Folder mF_Binaries;
            File::Folder mF_Product;
            File::Folder mF_Lib_Cfg;
            File::Folder mF_Libraries;

            // ===== Configurable attributes ================================
            DI::Enum<ComponentType, COMPONENT_TYPE_NAMES> mComponentType;

        };

    }
}
