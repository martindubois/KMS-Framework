
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/InstanceList.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <iostream>

namespace KMS
{
    namespace CLI
    {

        template <typename T>
        class InstanceList
        {

        public:

            InstanceList();

            ~InstanceList();

            void Add(const char* aName, T* aInstance);

            const T* Get(const char* aName) const;

            T* Get(const char* aName);

            const T* GetSelected(const char* aName) const;

            T* GetSelected();

            int Delete(const char* aName);

            void DeleteAll();

            int ExecuteCommand(const char* aCmd);

            void List(std::ostream& aOut) const;

            T* Remove(const char* aName);

            T* Select(const char* aName);

        private:

            int Cmd_Delete(const char* aName);
            int Cmd_Delete_All();
            int Cmd_List() const;
            int Cmd_Select(const char* aName);

            typedef std::map<std::string, T*> InstanceMap;

            InstanceMap mInstances;

            T* mSelected;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        InstanceList<T>::InstanceList() : mSelected(nullptr) {}

        template <typename T>
        InstanceList<T>::~InstanceList() { DeleteAll(); }

        template <typename T>
        void InstanceList<T>::Add(const char* aName, T* aInstance)
        {
            assert(nullptr != aInstance);

            mInstances.insert(InstanceMap::value_type(aName, aInstance));
        }

        template <typename T>
        T* InstanceList<T>::Get(const char* aName)
        {
            assert(nullptr != aName);

            auto lIt = mInstances.find(aName);
            if (mInstances.end() == lIt)
            {
                return nullptr;
            }

            assert(nullptr != lIt->second);

            return lIt->second;
        }

        template <typename T>
        T* InstanceList<T>::GetSelected() { return mSelected; }

        template <typename T>
        int InstanceList<T>::Delete(const char* aName)
        {
            int lResult = -2;

            auto lInstance = Remove(aName);
            if (nullptr != lInstance)
            {
                delete lInstance;

                lResult = 0;
            }

            return lResult;
        }

        template <typename T>
        void InstanceList<T>::DeleteAll()
        {
            for (auto lIt : mInstances)
            {
                assert(nullptr != lIt.second);

                delete lIt.second;
            }

            mInstances.clear();
            mSelected = nullptr;
        }

        template <typename T>
        int InstanceList<T>::ExecuteCommand(const char* aCmd)
        {
            int lResult = -4;

            char lName[NAME_LENGTH];

            if      (0 == _stricmp(aCmd, "Delete All")) { Cmd_Delete_All(); lResult = 0; }
            else if (0 == _stricmp(aCmd, "List"      )) { Cmd_List      (); lResult = 0; }
            else if (1 == sscanf_s(aCmd, "Delete %s", lName SizeInfo(lName))) { lResult = Cmd_Delete(lName); }
            else if (1 == sscanf_s(aCmd, "Select %s", lName SizeInfo(lName))) { lResult = Cmd_Select(lName); }
            else
            {
                std::cerr << "Invalid command" << std::endl;
            }

            return lResult;
        }

        template <typename T>
        void InstanceList<T>::List(std::ostream& aOut) const
        {
            for (auto lIt : mInstances)
            {
                aOut << lIt.first << "\n";
            }
        }

        template <typename T>
        T* InstanceList<T>::Remove(const char* aName)
        {
            assert(nullptr != aName);

            T* lResult = nullptr;

            auto lIt = mInstances.find(aName);
            if (mInstances.end() != lIt)
            {
                assert(nullptr != lIt->second);

                lResult = lIt->second;

                mInstances.erase(lIt);
            }

            if (mSelected == lResult)
            {
                mSelected = nullptr;
            }

            return lResult;
        }

        template <typename T>
        T* InstanceList<T>::Select(const char* aName)
        {
            mSelected = Get(aName);
            
            return mSelected;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        int InstanceList<T>::Cmd_Delete(const char* aName)
        {
            std::cout << aName << " ";

            int lResult = Delete(aName);
            if (0 == lResult)
            {
                std::cout << "deleted";
            }
            else
            {
                std::cerr << "does not exist";
            }

            std::cerr << std::endl;

            return lResult;
        }

        template <typename T>
        int InstanceList<T>::Cmd_Delete_All()
        {
            DeleteAll();

            std::cout << "Deleted" << std::endl;

            return 0;
        }

        template <typename T>
        int InstanceList<T>::Cmd_List() const
        {
            List(std::cout);

            std::cout << mInstances.size() << " instances" << std::endl;

            return 0;
        }

        template <typename T>
        int InstanceList<T>::Cmd_Select(const char* aName)
        {
            std::cout << aName << " ";

            int lResult = -3;

            auto lI = Select(aName);
            if (nullptr != lI)
            {
                std::cout << "selected";
                lResult = 0;
            }
            else
            {
                std::cerr << "does not exist";
            }

            std::cerr << std::endl;

            return lResult;
        }

    }
}
