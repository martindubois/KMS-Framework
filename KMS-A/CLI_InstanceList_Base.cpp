
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_InstanceList_Base.cpp

// TEST COVERAGE 2024-07-16 Martin Dubois

#include "Component.h"

// ===== C++ ================================================================
#include <regex>

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>

#include <KMS/CLI/InstanceList.h>

namespace KMS
{
    namespace CLI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int InstanceList_Base::CMD_DELETE     = 0x0001;
        const unsigned int InstanceList_Base::CMD_DELETE_ALL = 0x0002;
        const unsigned int InstanceList_Base::CMD_LIST       = 0x0004;
        const unsigned int InstanceList_Base::CMD_SELECT     = 0x0008;

        const unsigned int InstanceList_Base::CMD_ALL        = 0x000f;
        const unsigned int InstanceList_Base::CMD_NONE       = 0x0000;

        InstanceList_Base::InstanceList_Base() : mAllowedCmds(CMD_ALL) { Unselect(); }

        const char* InstanceList_Base::GetSelectedName() const
        {
            VerifyInstanceSelected();

            return mSelected->first.c_str();
        }

        bool InstanceList_Base::IsInstanceSelected() const { return mInstances.end() != mSelected; }

        void InstanceList_Base::SetAllowedCmds(unsigned int aCmds) { mAllowedCmds = aCmds; }

        void InstanceList_Base::DeleteAll()
        {
            for (auto lIt : mInstances)
            {
                assert(nullptr != lIt.second);

                Typed_Delete(lIt.second);
            }

            mInstances.clear();

            Unselect();
        }

        void InstanceList_Base::DeleteByName(const char* aName)
        {
            auto lInstance = Internal_RemoveByName(aName);

            Typed_Delete(lInstance);
        }

        void InstanceList_Base::DeleteSelected()
        {
            VerifyInstanceSelected();

            auto lInstance = Internal_RemoveSelected();

            Typed_Delete(lInstance);
        }

        int InstanceList_Base::ExecuteCommand(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            const char* lCmd = aCmd->GetCurrent();

            int lResult = __LINE__;

            if      (0 == _stricmp(lCmd, "Delete")) { aCmd->Next(); lResult = Cmd_Delete(aCmd); }
            else if (0 == _stricmp(lCmd, "List"  )) { aCmd->Next(); lResult = Cmd_List  (aCmd); }
            else if (0 == _stricmp(lCmd, "Select")) { aCmd->Next(); lResult = Cmd_Select(aCmd); }
            else
            {
                KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid command", lCmd);
            }

            return lResult;
        }

        void InstanceList_Base::List(std::ostream& aOut) const
        {
            for (auto lIt : mInstances)
            {
                aOut << lIt.first << "\n";
            }

            aOut << std::endl;
        }

        unsigned int InstanceList_Base::List(std::ostream& aOut, const char* aRegEx) const
        {
            assert(nullptr != aRegEx);

            unsigned int lResult = 0;

            std::regex lRegEx(aRegEx);

            for (auto lIt : mInstances)
            {
                if (std::regex_match(lIt.first, lRegEx))
                {
                    aOut << lIt.first << "\n";

                    lResult++;
                }
            }

            aOut << std::endl;

            return lResult;
        }

        void InstanceList_Base::VerifyInstanceSelected() const
        {
            KMS_EXCEPTION_ASSERT(IsInstanceSelected(), RESULT_INVALID_STATE, "No selected instance", "");
        }

        void InstanceList_Base::Unselect() { mSelected = mInstances.end(); }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void InstanceList_Base::Internal_Add(const char* aName, void* aInstance)
        {
            assert(nullptr != aName);
            assert(nullptr != aInstance);

            auto lRet = mInstances.insert(InstanceMap::value_type(aName, aInstance));
            if (!lRet.second)
            {
                Typed_Delete(aInstance);

                KMS_EXCEPTION(RESULT_INVALID_NAME, "Instance name already exist", aName);
            }

            assert(mInstances.end() != lRet.first);

            mSelected = lRet.first;
        }

        const void* InstanceList_Base::Internal_GetByName(const char* aName) const
        {
            assert(nullptr != aName);

            auto lIt = mInstances.find(aName);
            KMS_EXCEPTION_ASSERT(mInstances.end() != lIt, RESULT_INVALID_NAME, "Instance name does not exist", aName);

            assert(nullptr != lIt->second);

            return lIt->second;
        }

        void* InstanceList_Base::Internal_GetByName(const char* aName)
        {
            assert(nullptr != aName);

            auto lIt = mInstances.find(aName);
            KMS_EXCEPTION_ASSERT(mInstances.end() != lIt, RESULT_INVALID_NAME, "Instance name does not exist", aName);

            assert(nullptr != lIt->second);

            return lIt->second;
        }

        const void* InstanceList_Base::Internal_GetByName_Try(const char* aName) const
        {
            assert(nullptr != aName);

            void* lResult = nullptr;

            auto lIt = mInstances.find(aName);
            if (mInstances.end() != lIt)
            {
                assert(nullptr != lIt->second);

                lResult = lIt->second;
            }

            return lResult;
        }

        void* InstanceList_Base::Internal_GetByName_Try(const char* aName)
        {
            assert(nullptr != aName);

            void* lResult = nullptr;

            auto lIt = mInstances.find(aName);
            if (mInstances.end() != lIt)
            {
                assert(nullptr != lIt->second);

                lResult = lIt->second;
            }

            return lResult;
        }

        void* InstanceList_Base::Internal_GetSelected()
        {
            VerifyInstanceSelected();

            assert(nullptr != mSelected->second);

            return mSelected->second;
        }

        void InstanceList_Base::Internal_Delete(void* aInstance)
        {
            Internal_Remove(aInstance);

            Typed_Delete(aInstance);
        }

        void InstanceList_Base::Internal_Remove(void* aInstance)
        {
            assert(nullptr != aInstance);

            for (auto lIt = mInstances.begin(); lIt != mInstances.end(); lIt++)
            {
                assert(nullptr != lIt->second);

                if (aInstance == lIt->second)
                {
                    Unselect(aInstance);

                    mInstances.erase(lIt);

                    return;
                }
            }

            KMS_EXCEPTION(RESULT_INVALID_VALUE, "Unknown instance", "");
        }

        void* InstanceList_Base::Internal_RemoveByName(const char* aName)
        {
            assert(nullptr != aName);

            auto lIt = mInstances.find(aName);
            KMS_EXCEPTION_ASSERT(mInstances.end() != lIt, RESULT_INVALID_NAME, "The instance does not exist", aName);

            assert(nullptr != lIt->second);

            void* lResult = lIt->second;

            Unselect(lResult);

            mInstances.erase(lIt);

            return lResult;
        }

        void* InstanceList_Base::Internal_RemoveSelected()
        {
            VerifyInstanceSelected();

            assert(mInstances.end() != mSelected);
            assert(nullptr != mSelected->second);

            auto lResult = mSelected->second;

            mInstances.erase(mSelected);

            Unselect();

            return lResult;
        }

        void* InstanceList_Base::Internal_SelectByName(const char* aName)
        {
            assert(nullptr != aName);

            auto lIt = mInstances.find(aName);

            KMS_EXCEPTION_ASSERT(mInstances.end() != lIt, RESULT_INVALID_NAME, "The name does not exist", aName);

            assert(nullptr != lIt->second);

            mSelected = lIt;

            return mSelected->second;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        int InstanceList_Base::Cmd_Delete(CLI::CommandLine* aCmd)
        {
            int lResult = __LINE__;

            if (aCmd->IsAtEnd())
            {
                KMS_EXCEPTION_ASSERT(0 != (mAllowedCmds & CMD_DELETE), RESULT_DENIED, "Delete command is not allowed", "");

                DeleteSelected();

                std::cout << "Deleted" << std::endl;

                lResult = 0;
            }
            else
            {
                auto lCmd = aCmd->GetCurrent();

                if (0 == strcmp(lCmd, "All")) { aCmd->Next(); lResult = Cmd_Delete_All(aCmd); }
                else
                {
                    KMS_EXCEPTION(RESULT_INVALID_COMMAND, "Invalid command", lCmd);
                }
            }

            return lResult;
        }

        int InstanceList_Base::Cmd_Delete_All(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());
            KMS_EXCEPTION_ASSERT(0 != (mAllowedCmds & CMD_DELETE_ALL), RESULT_DENIED, "Delete All command is not allowed", "");

            DeleteAll();

            std::cout << "Deleted" << std::endl;

            return 0;
        }

        int InstanceList_Base::Cmd_List(CLI::CommandLine* aCmd) const
        {
            assert(nullptr != aCmd);

            KMS_EXCEPTION_ASSERT(0 != (mAllowedCmds & CMD_LIST), RESULT_DENIED, "List command is not allowed", "");

            if (aCmd->IsAtEnd())
            {
                List(std::cout);
            }
            else
            {
                char lText[LINE_LENGTH];

                aCmd->GetRemaining(lText, sizeof(lText));

                auto lCount = List(std::cout, lText);

                std::cout << lCount << " of ";
            }

            std::cout << mInstances.size() << " instances" << std::endl;

            return 0;
        }

        int InstanceList_Base::Cmd_Select(CLI::CommandLine* aCmd)
        {
            assert(nullptr != aCmd);

            auto lName = aCmd->GetCurrent();

            aCmd->Next();

            KMS_EXCEPTION_ASSERT(aCmd->IsAtEnd(), RESULT_INVALID_COMMAND, "Too many command arguments", aCmd->GetCurrent());
            KMS_EXCEPTION_ASSERT(0 != (mAllowedCmds & CMD_SELECT), RESULT_DENIED, "Select command is not allowed", "");

            Internal_SelectByName(lName);

            std::cout << lName << " selected" << std::endl;

            return 0;
        }

        void InstanceList_Base::Unselect(void* aInstance)
        {
            assert(nullptr != aInstance);

            if (IsInstanceSelected())
            {
                assert(nullptr != mSelected->second);

                if (mSelected->second == aInstance)
                {
                    Unselect();
                }
            }
        }

    }
}
