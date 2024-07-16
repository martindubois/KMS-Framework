
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/CLI/InstanceList.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <iostream>
#include <map>

namespace KMS
{
    namespace CLI
    {

        class CommandLine;

        class InstanceList_Base
        {

        public:

            static const unsigned int CMD_DELETE;
            static const unsigned int CMD_DELETE_ALL;
            static const unsigned int CMD_LIST;
            static const unsigned int CMD_SELECT;

            static const unsigned int CMD_NONE;
            static const unsigned int CMD_ALL;

            InstanceList_Base();

            // Exception  RESULT_INVALID_STATE
            const char* GetSelectedName() const;

            bool IsInstanceSelected() const;

            // aCmds  See CMD_...
            void SetAllowedCmds(unsigned int aCmds);

            void DeleteAll();

            // Exception  RESULT_INVALID_NAME
            void DeleteByName(const char* aName);

            // Exception  RESULT_INVALID_STATE
            void DeleteSelected();

            // Exception  RESULT_DENIED
            //            RESULT_INVALID_COMMAND
            //            RESULT_INVALID_NAME
            //            RESULT_INVALID_STATE
            //            RESULT_INVALID_VALUE
            int ExecuteCommand(CLI::CommandLine* aCmd);

            void List(std::ostream& aOut) const;

            // Exception  RESULT_INVALID_STATE
            void VerifyInstanceSelected() const;

            void Unselect();

            typedef std::map<std::string, void*> InstanceMap;

            InstanceMap mInstances;

        protected:

            void Internal_Add(const char* aName, void* aInstance);

            const void* Internal_GetByName(const char* aName) const;

            void* Internal_GetByName(const char* aName);

            const void* Internal_GetByName_Try(const char* aName) const;

            void* Internal_GetByName_Try(const char* aName);

            void* Internal_GetSelected();

            void Internal_Delete(void* aInstance);

            void Internal_Remove(void* aInstance);

            void* Internal_RemoveByName(const char* aName);

            void* Internal_RemoveSelected();

            void* Internal_SelectByName(const char* aName);

            virtual void Typed_Delete(void* aInstance) = 0;

            unsigned int mAllowedCmds;

            InstanceMap::iterator mSelected;

        private:

            int Cmd_Delete    (CLI::CommandLine* aCmd);
            int Cmd_Delete_All(CLI::CommandLine* aCmd);
            int Cmd_List      (CLI::CommandLine* aCmd) const;
            int Cmd_Select    (CLI::CommandLine* aCmd);

            void Unselect(void* aInstance);

        };

        template <typename T>
        class InstanceList : public InstanceList_Base
        {

        public:

            ~InstanceList();

            // Exception  RESULT_INVALID_NAME
            void Add(const char* aName, T* aInstance);

            // Exception  RESULT_INVALID_NAME
            const T* GetByName(const char* aName) const;

            // Exception  RESULT_INVALID_NAME
            T* GetByName(const char* aName);

            const T* GetByName_Try(const char* aName) const;

            T* GetByName_Try(const char* aName);

            // Exception  RESULT_INVALID_STATE
            const T* GetSelected() const;

            // Exception  RESULT_INVALID_STATE
            T* GetSelected();

            // Exception  RESULT_INVALID_VALUE
            void Delete(T* aInstance);

            // Exception  RESULT_INVALID_VALUE
            void Remove(T* aInstance);

            // Exception  RESULT_INVALID_NAME
            T* RemoveByName(const char* aName);

            // Exception  RESULT_INVALID_STATE
            T* RemoveSelected();

            // Exception  RESULT_INVALID_NAME
            T* SelectByName(const char* aName);

        protected:

            // ===== InstanceList_Base ======================================
            virtual void Typed_Delete(void* aInstance);

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        InstanceList<T>::~InstanceList() { DeleteAll(); }

        template <typename T>
        void InstanceList<T>::Add(const char* aName, T* aInstance)
        {
            Internal_Add(aName, aInstance);
        }

        template <typename T>
        const T* InstanceList<T>::GetByName(const char* aName) const
        {
            return reinterpret_cast<T*>(Internal_GetByName(aName));
        }

        template <typename T>
        T* InstanceList<T>::GetByName(const char* aName)
        {
            return reinterpret_cast<T*>(Internal_GetByName(aName));
        }

        template <typename T>
        const T* InstanceList<T>::GetByName_Try(const char* aName) const
        {
            return reinterpret_cast<T*>(Internal_GetByName_Try(aName));
        }

        template <typename T>
        T* InstanceList<T>::GetByName_Try(const char* aName)
        {
            return reinterpret_cast<T*>(Internal_GetByName_Try(aName));
        }

        template <typename T>
        const T* InstanceList<T>::GetSelected() const { return reinterpret_cast<T*>(Internal_GetSelected()); }

        template <typename T>
        T* InstanceList<T>::GetSelected() { return reinterpret_cast<T*>(Internal_GetSelected()); }

        template <typename T>
        void InstanceList<T>::Delete(T* aInstance)
        {
            Internal_Delete(aInstance);
        }

        template <typename T>
        void InstanceList<T>::Remove(T* aInstance)
        {
            Internal_Remove(aInstance);
        }

        template <typename T>
        T* InstanceList<T>::RemoveByName(const char* aName)
        {
            return reinterpret_cast<T*>(Internal_RemoveByName(aName));
        }

        template <typename T>
        T* InstanceList<T>::RemoveSelected()
        {
            return reinterpret_cast<T*>(Internal_RemoveSelected());
        }

        template <typename T>
        T* InstanceList<T>::SelectByName(const char* aName)
        {
            return reinterpret_cast<T*>(Internal_SelectByName(aName));
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // ===== InstanceList_Base ==========================================

        template <typename T>
        void InstanceList<T>::Typed_Delete(void* aInstance)
        {
            assert(nullptr != aInstance);

            delete reinterpret_cast<T*>(aInstance);
        }

    }
}
