
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Exception_W.cpp

#include "Component.h"

// ====== Windows ===========================================================
#include <Windows.h>

KMS_RESULT_STATIC(RESULT_ACCESS_VIOLATION);
KMS_RESULT_STATIC(RESULT_INTEGER_DIVIDE_BY_ZERO);
KMS_RESULT_STATIC(RESULT_STRUCTURED_EXCEPTION);

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

// ===== Entry point ========================================================
static void TranslateException(unsigned int aCode, struct _EXCEPTION_POINTERS* aStruct);

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void* Exception::RegisterTranslator()
    {
        return _set_se_translator(TranslateException);
    }

    void Exception::RestoreTranslator(void* aTranslator)
    {
        auto lTranslator = _set_se_translator(reinterpret_cast<_se_translator_function>(aTranslator));
        assert(TranslateException == lTranslator);
    }

    // Private
    // //////////////////////////////////////////////////////////////////////

    void Exception::Construct_OSDep()
    {
        mLastError = ::GetLastError();
    }

}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

// ===== Entry point ========================================================
void TranslateException(unsigned int aCode, struct _EXCEPTION_POINTERS* aStruct)
{
    const char* lM;
    Result      lR;

    switch (aCode)
    {
    case 0xc0000005: lR = RESULT_ACCESS_VIOLATION      ; lM = "Access violation"   ; break;
    case 0xc0000094: lR = RESULT_INTEGER_DIVIDE_BY_ZERO; lM = "Integer divide by 0"; break;

    default: lR = RESULT_STRUCTURED_EXCEPTION; lM = "Structured exception";
    }

    KMS_EXCEPTION(lR, lM, aCode);
}
