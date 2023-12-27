
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WGDI_Scope_Module.cpp

#include "Component.h"

// ===== Includes ===========================================================
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

        int Scope_Module::ExecuteCommand(const char* aC)
        {
            int lResult = CLI::Module::UNKNOWN_COMMAND;

            char lC[LINE_LENGTH];

            if (1 == sscanf_s(aC, "Scope %[^\n\r]", lC SizeInfo(lC)))
            {
                lResult = 0;

                double       lDouble;
                unsigned int lUInt;
                char         lStr[NAME_LENGTH];

                if      (0 == strcmp(lC, "Channel Random")) { Channel_Random(); }
                else if (0 == strcmp(lC, "Clear"         )) { Clear         (); }
                else if (0 == strcmp(lC, "Delete"        )) { Delete        (); }
                else if (0 == strcmp(lC, "ForceTrig"     )) { ForceTrig     (); }
                else if (0 == strcmp(lC, "Info"          )) { Info          (); }
                else if (0 == strcmp(lC, "Show"          )) { Show          (); }
                else if (0 == strcmp(lC, "Start"         )) { Start         (); }
                else if (0 == strcmp(lC, "Stop"          )) { Stop          (); }
                else if (1 == sscanf_s(lC, "Channel Color %[A-Z]"    , lStr SizeInfo(lStr))) { Channel_Color   (lStr); }
                else if (1 == sscanf_s(lC, "Channel EdgeType %[A-Z_]", lStr SizeInfo(lStr))) { Channel_EdgeType(lStr); }
                else if (1 == sscanf_s(lC, "Mode %[A-Z_]"            , lStr SizeInfo(lStr))) { Mode            (lStr); }
                else if (1 == sscanf_s(lC, "Select %s"               , lStr SizeInfo(lStr))) { Select          (lStr); }
                else if (1 == sscanf_s(lC, "Channel ScaleY %lf"   , &lDouble)) { Channel_ScaleY   (lDouble); }
                else if (1 == sscanf_s(lC, "Channel TrigLevel %lf", &lDouble)) { Channel_TrigLevel(lDouble); }
                else if (1 == sscanf_s(lC, "Channel Y %u"  , &lUInt)) { Channel_Y  (lUInt); }
                else if (1 == sscanf_s(lC, "Frequency %u"  , &lUInt)) { Frequency  (lUInt); }
                else if (1 == sscanf_s(lC, "Persistence %u", &lUInt)) { Persistence(lUInt); }
                else if (1 == sscanf_s(lC, "PositionX %u"  , &lUInt)) { PositionX  (lUInt); }
                else if (1 == sscanf_s(lC, "ScaleX %u"     , &lUInt)) { ScaleX     (lUInt); }
                else
                {
                    lResult = UNKNOWN_COMMAND;
                }
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Commands ===================================================

        void Scope_Module::Clear()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->mScope.Clear();
        }

        void Scope_Module::Channel_Color(const char* aColor)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            Graph::Color lColor(Graph::Color::BLUE);

            if      (0 == strcmp(aColor, "BLUE" )) { lColor = Graph::Color::BLUE; }
            else if (0 == strcmp(aColor, "GREEN")) { lColor = Graph::Color::GREEN; }
            else if (0 == strcmp(aColor, "RED"  )) { lColor = Graph::Color::RED; }
            else if (0 == strcmp(aColor, "WHITE")) { lColor = Graph::Color::WHITE; }
            else
            {
                KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid color name", aColor);
            }

            mChannel->mColor = lColor;
        }

        void Scope_Module::Channel_EdgeType(const char* aEdgeType)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            Enum<KMS::Scope::Channel::EdgeType, KMS::Scope::Channel::EDGE_TYPE_NAMES> lEdgeType(mChannel->mEdgeType);

            lEdgeType.SetName(aEdgeType);

            mChannel->mEdgeType = lEdgeType;
        }

        void Scope_Module::Channel_Random()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            AddChannel(new KMS::Scope::Channel_Random);
        }

        void Scope_Module::Channel_ScaleY(double aScaleY)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            mChannel->mScaleY = aScaleY;
        }

        void Scope_Module::Channel_TrigLevel(double aTrigLevel)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            mChannel->mTrigLevel = aTrigLevel;
        }

        void Scope_Module::Channel_Y(unsigned int aY_px)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mChannel, RESULT_INVALID_STATE, "No selected channel", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Channel::Y_MAX_px >= aY_px, RESULT_INVALID_VALUE, "Invalif channel Y", aY_px);

            mChannel->mY_px = aY_px;
        }

        void Scope_Module::Delete()
        {
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
        }

        void Scope_Module::Frequency(unsigned int aFrequency_Hz)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::FREQUENCY_MAX_Hz >= aFrequency_Hz, RESULT_INVALID_CONFIG, "The frequency is above the maximum", aFrequency_Hz);
            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::FREQUENCY_MIN_Hz <= aFrequency_Hz, RESULT_INVALID_CONFIG, "The frequency is belo the minimum" , aFrequency_Hz);

            mScope->mScope.mFrequency_Hz = aFrequency_Hz;
        }

        void Scope_Module::ForceTrig()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->mScope.ForceTrig();
        }

        void Scope_Module::Info()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            auto& lOut = mConsole->OutputStream();

            lOut << mScope->mScope.Channels_GetCount() << " channels\n";
            lOut << "Sample frequency : " << mScope->mScope.mFrequency_Hz << " Hz\n";
            lOut << "Mode             : " << mScope->mScope.GetModeName() << "\n";
            lOut << "Position X       : " << mScope->mScope.mPositionX_px << " px\n";
            lOut << "Scale X          : " << mScope->mScope.mScaleX_ms_px << " ms/px\n";
            lOut << "----- Inter sample time (ms) -----\n";

            lOut << mScope->mScope.mStats_InterSample << std::endl;
        }

        void Scope_Module::Mode(const char* aMode)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            Enum<KMS::Scope::Scope::Mode, KMS::Scope::Scope::MODE_NAMES> lMode(mScope->mScope.mMode);

            lMode.SetName(aMode);

            mScope->mScope.mMode = lMode;
        }

        void Scope_Module::Persistence(unsigned int aPersistence)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::PERSISTENCE_MIN <= aPersistence, RESULT_INVALID_CONFIG, "The persistence is below the minimum", aPersistence);

            mScope->mScope.mPersistence = aPersistence;
        }

        void Scope_Module::PositionX(unsigned int aPositionX_px)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::POSITION_X_MAX_px >= aPositionX_px, RESULT_INVALID_CONFIG, "The position is above the maximum", aPositionX_px);

            mScope->mScope.mPositionX_px = aPositionX_px;
        }

        void Scope_Module::ScaleX(unsigned int aScaleX_ms_px)
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::SCALE_X_MAX_ms_px >= aScaleX_ms_px, RESULT_INVALID_CONFIG, "The scale is above the maximum", aScaleX_ms_px);
            KMS_EXCEPTION_ASSERT(KMS::Scope::Scope::SCALE_X_MIN_ms_px <= aScaleX_ms_px, RESULT_INVALID_CONFIG, "The scale is belo the minimum" , aScaleX_ms_px);

            mScope->mScope.mScaleX_ms_px = aScaleX_ms_px;
        }

        void Scope_Module::Select(const char* aName)
        {
            assert(nullptr != aName);

            auto lIt = mScopes.find(aName);
            if (mScopes.end() == lIt)
            {
                char lTitle[NAME_LENGTH];

                sprintf_s(lTitle, "Scope - %s", aName);

                mScope = new Scope(lTitle);

                mScopes.insert(ScopeMap::value_type(aName, mScope));
            }
            else
            {
                assert(nullptr != lIt->second);

                mScope = lIt->second;
            }
        }

        void Scope_Module::Show()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->Show();
        }

        void Scope_Module::Start()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->Start();
        }

        void Scope_Module::Stop()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mScope, RESULT_INVALID_STATE, "No selected scope", "");

            mScope->Stop();
        }

    }
}
