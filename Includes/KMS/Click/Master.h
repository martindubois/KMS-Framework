
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Click/Master.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <vector>

namespace KMS
{
    /// \brief Experimental
    namespace Click
    {

        class Master
        {

        public:

            enum class KeyCode
            {
                F1, F10,
                RETURN,
                TAB,

                QTY
            };

            static const unsigned int FLAG_ALT;
            static const unsigned int FLAG_DESKTOP;
            static const unsigned int FLAG_NEXT;
            static const unsigned int FLAG_NO_EXCEPTION;
            static const unsigned int FLAG_PUSH;
            static const unsigned int FLAG_RECURSE;
            static const unsigned int FLAG_RETRY;

            Master();

            virtual ~Master();

            void SetDelay(unsigned int aDelay_ms);

            void Key(KeyCode aKey, unsigned int aFlags = 0);
            void Key(char    aKey, unsigned int aFlags = 0);

            void Message_Close();

            // aFlags  FLAG_ALT
            void SystemKey(KeyCode aKey, unsigned int aFlags = 0);
            void SystemKey(char    aKey, unsigned int aFlags = 0);

            void Text(char aKey);
            void Text(const char* aText);

            void Window_Pop();

            // aFlags
            //  FLAG_DESKTOP       Ignored when FLAG_NEXT is present
            //  FLAG_NEXT          Do not use with FLAG_RECURSE
            //  FLAG_NO_EXCEPTION
            //  FLAG_PUSH
            //  FLAG_RECURSE       Do not use with FLAG_NEXT
            //  FLAG_RETRY
            bool Window_Select(const char* aClass, const char* aName, unsigned int aFlags = 0);
            bool Window_Select(HWND aHandle, unsigned int aFlags = 0);

        private:

            class FlagData
            {

            public:

                FlagData();

                HWND         mCurrent;
                bool         mException;
                HWND         mParent;
                bool         mRecurse;
                unsigned int mRetryCount;
               
            };

            NO_COPY(Master);

            void Key(unsigned int aMsg, char aKey, unsigned int aFlags);

            void Message_Post(unsigned int aMsg, uint64_t aParamW = 0, uint64_t aParamL = 0);

            void ParseFlags(unsigned int aFlags, FlagData* aOut);

            HWND            mCurrent;
            unsigned int    mDelay_ms;
            std::list<HWND> mStack;

        };

    }
}
