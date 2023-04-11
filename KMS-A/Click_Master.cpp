
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Click_Master.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Click/Master.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define RETRY_COUNT    (3)
#define RETRY_DELAY_ms (400)

#define TEXT_DELAY_ms (500)

// Data types
// //////////////////////////////////////////////////////////////////////////

class Operation
{

public:

    Operation(const char* aClass, const char* aName);

    HWND GetResult();

    BOOL Search(HWND aHandle);

private:

    const char* mClass;
    const char* mName;
    HWND        mResult;

};

// Constants
// //////////////////////////////////////////////////////////////////////////

static const char KEY_CODES[static_cast<unsigned int>(KMS::Click::Master::KeyCode::QTY)] =
{
    VK_F1, VK_F10,
    VK_RETURN,
    VK_TAB,
};

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static BOOL CALLBACK Operation_Search(HWND aHandle, LPARAM aParam);

namespace KMS
{
    namespace Click
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int Master::FLAG_ALT          = 0x00000001;
        const unsigned int Master::FLAG_DESKTOP      = 0x00000002;
        const unsigned int Master::FLAG_NEXT         = 0x00000004;
        const unsigned int Master::FLAG_NO_EXCEPTION = 0x00000008;
        const unsigned int Master::FLAG_PUSH         = 0x00000010;
        const unsigned int Master::FLAG_RECURSE      = 0x00000020;
        const unsigned int Master::FLAG_RETRY        = 0x00000040;

        Master::Master() : mCurrent(NULL), mDelay_ms(0) {}

        Master::~Master() {}

        void Master::SetDelay(unsigned int aDelay_ms) { mDelay_ms = aDelay_ms; }

        void Master::Key(KeyCode aKey, unsigned int aFlags)
        {
            KMS_EXCEPTION_ASSERT(KeyCode::QTY > aKey, CLICK_KEY_INVALID, "Invalid key", "");

            char lKey = KEY_CODES[static_cast<unsigned int>(aKey)];

            Key(WM_KEYDOWN, lKey, aFlags);
            Key(WM_KEYUP  , lKey, aFlags);
        }

        void Master::Key(char aKey, unsigned int aFlags)
        {
            char lKey = toupper(aKey);

            Key(WM_KEYDOWN, lKey, aFlags);
            Key(WM_KEYUP  , lKey, aFlags);
        }

        void Master::Message_Close() { Message_Post(WM_CLOSE); }

        void Master::SystemKey(KeyCode aKey, unsigned int aFlags)
        {
            KMS_EXCEPTION_ASSERT(KeyCode::QTY > aKey, CLICK_KEY_INVALID, "Invalid key", "");

            char lKey = KEY_CODES[static_cast<unsigned int>(aKey)];

            Key(WM_SYSKEYDOWN, lKey, aFlags);
            Key(WM_SYSKEYUP  , lKey, aFlags);
        }

        void Master::SystemKey(char aKey, unsigned int aFlags)
        {
            char lKey = toupper(aKey);

            Key(WM_SYSKEYDOWN, lKey, aFlags);
            Key(WM_SYSKEYUP  , lKey, aFlags);
        }

        void Master::Text(char aKey) { Message_Post(WM_CHAR, aKey); }

        void Master::Text(const char* aText)
        {
            assert(NULL != aText);

            Sleep(TEXT_DELAY_ms);

            const char* lText = aText;
            while ('\0' != *lText)
            {
                Text(*lText);
                lText++;
            }
        }

        void Master::Window_Pop()
        {
            KMS_EXCEPTION_ASSERT(1 <= mStack.size(), CLICK_OPERATION_FAILED, "No window to pop", "");

            mCurrent = mStack.back();
            assert(NULL != mCurrent);

            mStack.pop_back();
        }

        bool Master::Window_Select(const char* aClass, const char* aName, unsigned int aFlags)
        {
            assert((NULL != aClass) || (NULL != aName));

            FlagData lFD;

            ParseFlags(aFlags, &lFD);

            for (unsigned int lRetry = 0; lRetry < lFD.mRetryCount; lRetry++)
            {
                HWND lHandle = FindWindowEx(lFD.mParent, lFD.mCurrent, aClass, aName);
                if (NULL != lHandle)
                {
                    return Window_Select(lHandle, aFlags);
                }

                Sleep(RETRY_DELAY_ms);
            }

            if (lFD.mRecurse)
            {
                Operation lOperation(aClass, aName);

                EnumChildWindows(lFD.mParent, Operation_Search, reinterpret_cast<LPARAM>(&lOperation));

                HWND lHandle = lOperation.GetResult();
                if (NULL != lHandle)
                {
                    return Window_Select(lHandle, aFlags);
                }
            }

            KMS_EXCEPTION_ASSERT(lFD.mException, CLICK_CLASS_INVALID, "Invalid window selection", "");
            return false;
        }

        bool Master::Window_Select(HWND aHandle, unsigned int aFlags)
        {
            assert(NULL != aHandle);

            if ((NULL != mCurrent) && (0 != (aFlags & FLAG_PUSH)))
            {
                mStack.push_back(mCurrent);
            }

            mCurrent = aHandle;

            return true;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Master::Key(unsigned int aMsg, char aKey, unsigned int aFlags)
        {
            uint64_t lParamL = (0 == (aFlags & FLAG_ALT) ? 0 : (KF_ALTDOWN << 16));

            switch (aMsg)
            {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN: break;

            case WM_KEYUP:
            case WM_SYSKEYUP: lParamL |= 0xc0000000; break;

            default: assert(false);
            }

            Message_Post(aMsg, aKey, lParamL);
        }

        void Master::Message_Post(unsigned int aMsg, uint64_t aParamW, uint64_t aParamL)
        {
            assert(NULL != mCurrent);

            Sleep(mDelay_ms);

            BOOL lRet = PostMessage(mCurrent, aMsg, aParamW, aParamL);
            KMS_EXCEPTION_ASSERT(lRet, CLICK_OPERATION_FAILED, "Cannot post message", "");
        }

        void Master::ParseFlags(unsigned int aFlags, FlagData* aOut)
        {
            assert(NULL != aOut);

            if (0 == (aFlags & FLAG_NEXT))
            {
                if (0 == (aFlags & FLAG_DESKTOP)) { aOut->mParent = mCurrent; }
            }
            else
            {
                KMS_EXCEPTION_ASSERT(NULL != mCurrent , CLICK_OPERATION_FAILED, "Invalid use of FLAG_NEXT", "");
                KMS_EXCEPTION_ASSERT(0 < mStack.size(), CLICK_OPERATION_FAILED, "Invalid use of FLAG_NEXT", "");

                aOut->mCurrent = mCurrent;
                aOut->mParent  = mStack.back();

                assert(NULL != aOut->mParent);
            }

            if (0 != (aFlags & FLAG_NO_EXCEPTION)) { aOut->mException  = false; }
            if (0 != (aFlags & FLAG_RECURSE     )) { aOut->mRecurse    = true; }
            if (0 != (aFlags & FLAG_RETRY       )) { aOut->mRetryCount = RETRY_COUNT; }
        }

        Master::FlagData::FlagData() : mCurrent(NULL), mException(true), mParent(NULL), mRecurse(false), mRetryCount(1) {}

    }
}

using namespace KMS;

Operation::Operation(const char* aClass, const char* aName) : mClass(aClass), mName(aName), mResult(NULL)
{
    assert((NULL != aClass) || (NULL != aName));
}

HWND Operation::GetResult() { return mResult; }

BOOL Operation::Search(HWND aHandle)
{
    assert(NULL != aHandle);

    assert((NULL != mClass) || (NULL != mName));
    assert(NULL == mResult);

    mResult = FindWindowEx(aHandle, NULL, mClass, mName);
    if (NULL == mResult)
    {
        EnumChildWindows(aHandle, Operation_Search, reinterpret_cast<LPARAM>(this));
    }

    return (NULL == mResult) ? TRUE : FALSE;
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

BOOL Operation_Search(HWND aHandle, LPARAM aParam)
{
    assert(NULL != aParam);

    Operation* lOperation = reinterpret_cast<Operation*>(aParam);

    return lOperation->Search(aHandle);
}
