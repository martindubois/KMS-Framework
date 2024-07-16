
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WGDI_Scope_Module.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>
#include <KMS/Convert.h>
#include <KMS/Scope/Channel_Random.h>

#include <KMS/WGDI/Scope_Module.h>

namespace KMS
{
    namespace WGDI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Scope_Module::Scope_Module() : mChannel(nullptr), mScope(nullptr) {}

        Scope_Module::~Scope_Module()
        {
            for (auto lScope : mScopes)
            {
                assert(nullptr != lScope.second);

                delete lScope.second;
            }
        }

        void Scope_Module::AddChannel(KMS::Scope::Channel* aChannel)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mChannel = aChannel;

            mScope->AddChannel(aChannel);
        }

        // ===== CLI::Module ================================================

        // TODO  Add commands
        //          Scope Channel Color
        //                        ScaleY
        //                        TrigLevel
        //                        Y
        void Scope_Module::DisplayHelp(FILE* aOut) const
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "Scope Channel Color BLUE | GREEN | RED | WHITE\n"
                "Scope Channel EdgeType BOTH | FALLING | NONE | RAISING\n"
                "Scope Channel Random\n"
                "Scope Channel ScaleY {Value}\n"
                "Scope Channel TrigLevel {Value}\n"
                "Scope Channel Y {px}\n"
                "Scope Clear\n"
                "Scope Delete\n"
                "Scope ForceTrig\n"
                "Scope Frequency {Hz}\n"
                "Scope Info\n"
                "Scope Mode AUTO | CONTINUOUS | CONTINUOUS_SCROLLING | NORMAL | SINGLE | SINGLE_SCROLLING\n"
                "Scope Persistence {Value}\n"
                "Scope PositionX {px}\n"
                "Scope ScaleX {us/px}\n"
                "Scope Select {Name}\n"
                "Scope Show\n"
                "Scope Start\n"
                "Scope Stop\n");
        }

        int Scope_Module::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = CLI::Module::UNKNOWN_COMMAND;

            if (0 == _stricmp(lCmd, "Scope")) { aCmd->Next(); lResult = Cmd(aCmd); }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Commands ===================================================

        int Scope_Module::Cmd(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if      (0 == _stricmp(lCmd, "Channel"    )) { aCmd->Next(); lResult = Cmd_Channel    (aCmd); }
            else if (0 == _stricmp(lCmd, "Clear"      )) { aCmd->Next(); lResult = Cmd_Clear      (aCmd); }
            else if (0 == _stricmp(lCmd, "Delete"     )) { aCmd->Next(); lResult = Cmd_Delete     (aCmd); }
            else if (0 == _stricmp(lCmd, "ForceTrig"  )) { aCmd->Next(); lResult = Cmd_ForceTrig  (aCmd); }
            else if (0 == _stricmp(lCmd, "Frequency"  )) { aCmd->Next(); lResult = Cmd_Frequency  (aCmd); }
            else if (0 == _stricmp(lCmd, "Info"       )) { aCmd->Next(); lResult = Cmd_Info       (aCmd); }
            else if (0 == _stricmp(lCmd, "Mode"       )) { aCmd->Next(); lResult = Cmd_Mode       (aCmd); }
            else if (0 == _stricmp(lCmd, "Persistence")) { aCmd->Next(); lResult = Cmd_Persistence(aCmd); }
            else if (0 == _stricmp(lCmd, "PositionX"  )) { aCmd->Next(); lResult = Cmd_PositionX  (aCmd); }
            else if (0 == _stricmp(lCmd, "ScaleX"     )) { aCmd->Next(); lResult = Cmd_ScaleX     (aCmd); }
            else if (0 == _stricmp(lCmd, "Select"     )) { aCmd->Next(); lResult = Cmd_Select     (aCmd); }
            else if (0 == _stricmp(lCmd, "Show"       )) { aCmd->Next(); lResult = Cmd_Show       (aCmd); }
            else if (0 == _stricmp(lCmd, "Start"      )) { aCmd->Next(); lResult = Cmd_Start      (aCmd); }
            else if (0 == _stricmp(lCmd, "Stop"       )) { aCmd->Next(); lResult = Cmd_Stop       (aCmd); }
            else
            {
                lResult = UNKNOWN_COMMAND;
            }

            return lResult;
        }

        int Scope_Module::Cmd_Channel(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lCmd = aCmd->GetCurrent();
            auto lResult = UNKNOWN_COMMAND;

            if      (0 == _stricmp(lCmd, "Color"    )) { aCmd->Next(); lResult = Cmd_Channel_Color    (aCmd); }
            else if (0 == _stricmp(lCmd, "EdgeType" )) { aCmd->Next(); lResult = Cmd_Channel_EdgeType (aCmd); }
            else if (0 == _stricmp(lCmd, "Random"   )) { aCmd->Next(); lResult = Cmd_Channel_Random   (aCmd); }
            else if (0 == _stricmp(lCmd, "ScaleY"   )) { aCmd->Next(); lResult = Cmd_Channel_ScaleY   (aCmd); }
            else if (0 == _stricmp(lCmd, "TrigLevel")) { aCmd->Next(); lResult = Cmd_Channel_TrigLevel(aCmd); }
            else if (0 == _stricmp(lCmd, "Y"        )) { aCmd->Next(); lResult = Cmd_Channel_Y        (aCmd); }

            return lResult;
        }

        int Scope_Module::Cmd_Channel_Color(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lColorName = aCmd->GetCurrent();

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            Graph::Color lColor(Graph::Color::BLUE);

            if      (0 == _stricmp(lColorName, "BLUE" )) { lColor = Graph::Color::BLUE; }
            else if (0 == _stricmp(lColorName, "GREEN")) { lColor = Graph::Color::GREEN; }
            else if (0 == _stricmp(lColorName, "RED"  )) { lColor = Graph::Color::RED; }
            else if (0 == _stricmp(lColorName, "WHITE")) { lColor = Graph::Color::WHITE; }
            else
            {
                KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid color name", lColorName);
            }

            mChannel->mColor = lColor;

            return 0;
        }

        int Scope_Module::Cmd_Channel_EdgeType(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lEdgeTypeName = aCmd->GetCurrent();

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            Enum<KMS::Scope::Channel::EdgeType, KMS::Scope::Channel::EDGE_TYPE_NAMES> lEdgeType(mChannel->mEdgeType);

            lEdgeType.SetName(lEdgeTypeName);

            mChannel->mEdgeType = lEdgeType;

            return 0;
        }

        int Scope_Module::Cmd_Channel_Random(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            AddChannel(new KMS::Scope::Channel_Random);

            return 0;
        }

        int Scope_Module::Cmd_Channel_ScaleY(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lScaleY = Convert::ToDouble(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            mChannel->mScaleY = lScaleY;

            return 0;
        }

        int Scope_Module::Cmd_Channel_TrigLevel(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lTrigLevel = Convert::ToDouble(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            mChannel->mTrigLevel = lTrigLevel;

            return 0;
        }

        int Scope_Module::Cmd_Channel_Y(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lY_px = Convert::ToUInt16(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Channel::Y_MAX_px >= lY_px, RESULT_INVALID_VALUE, "Invalif channel Y", lY_px);

            mChannel->mY_px = lY_px;

            return 0;
        }

        int Scope_Module::Cmd_Clear(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->mScope.Clear();

            return 0;
        }

        int Scope_Module::Cmd_Delete(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            for (auto lScope : mScopes)
            {
                if (mScope == lScope.second)
                {
                    mScopes.erase(lScope.first);
                    break;
                }
            }

            delete mScope;
            mScope = nullptr;

            return 0;
        }

        int Scope_Module::Cmd_Frequency(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lFrequency_Hz = Convert::ToUInt32(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::FREQUENCY_MAX_Hz >= lFrequency_Hz, RESULT_INVALID_CONFIG, "The frequency is above the maximum", lFrequency_Hz);
            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::FREQUENCY_MIN_Hz <= lFrequency_Hz, RESULT_INVALID_CONFIG, "The frequency is belo the minimum" , lFrequency_Hz);

            mScope->mScope.mFrequency_Hz = lFrequency_Hz;

            return 0;
        }

        int Scope_Module::Cmd_ForceTrig(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->mScope.ForceTrig();

            return 0;
        }

        int Scope_Module::Cmd_Info(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            std::cout << mScope->mScope.Channels_GetCount() << " channels\n";
            std::cout << "Sample frequency : " << mScope->mScope.mFrequency_Hz << " Hz\n";
            std::cout << "Mode             : " << mScope->mScope.GetModeName() << "\n";
            std::cout << "Position X       : " << mScope->mScope.mPositionX_px << " px\n";
            std::cout << "Scale X          : " << mScope->mScope.mScaleX_ms_px << " ms/px\n";
            std::cout << "----- Inter sample time (ms) -----\n";

            std::cout << mScope->mScope.mStats_InterSample << std::endl;

            return 0;
        }

        int Scope_Module::Cmd_Mode(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            auto lModeName = aCmd->GetCurrent();

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            Enum<KMS::Scope::Scope::Mode, KMS::Scope::Scope::MODE_NAMES> lMode(mScope->mScope.mMode);

            lMode.SetName(lModeName);

            mScope->mScope.mMode = lMode;

            return 0;
        }

        int Scope_Module::Cmd_Persistence(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lPersistence = Convert::ToUInt32(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::PERSISTENCE_MIN <= lPersistence, RESULT_INVALID_CONFIG, "The persistence is below the minimum", lPersistence);

            mScope->mScope.mPersistence = lPersistence;

            return 0;
        }

        int Scope_Module::Cmd_PositionX(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lPositionX_px = Convert::ToUInt32(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::POSITION_X_MAX_px >= lPositionX_px, RESULT_INVALID_CONFIG, "The position is above the maximum", lPositionX_px);

            mScope->mScope.mPositionX_px = lPositionX_px;

            return 0;
        }

        int Scope_Module::Cmd_ScaleX(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lScaleX_ms_px = Convert::ToUInt32(aCmd->GetCurrent());

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::SCALE_X_MAX_ms_px >= lScaleX_ms_px, RESULT_INVALID_CONFIG, "The scale is above the maximum", lScaleX_ms_px);
            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::SCALE_X_MIN_ms_px <= lScaleX_ms_px, RESULT_INVALID_CONFIG, "The scale is belo the minimum" , lScaleX_ms_px);

            mScope->mScope.mScaleX_ms_px = lScaleX_ms_px;

            return 0;
        }

        int Scope_Module::Cmd_Select(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lName = aCmd->GetCurrent();

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            assert(nullptr != lName);

            auto lIt = mScopes.find(lName);
            if (mScopes.end() == lIt)
            {
                char lTitle[NAME_LENGTH];

                sprintf_s(lTitle, "Scope - %s", lName);

                mScope = new Scope(lTitle);

                mScopes.insert(ScopeMap::value_type(lName, mScope));
            }
            else
            {
                assert(nullptr != lIt->second);

                mScope = lIt->second;
            }

            return 0;
        }

        int Scope_Module::Cmd_Show(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->Show();

            return 0;
        }

        int Scope_Module::Cmd_Start(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->Start();

            return 0;
        }

        int Scope_Module::Cmd_Stop(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());

            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->Stop();

            return 0;
        }

    }
}
