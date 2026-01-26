
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2026 KMS
/// \file      Includes/KMS/Main2.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/ArgList.h>
#include <KMS/Console/Color.h>
#include <KMS/Exception.h>

// Macros
// //////////////////////////////////////////////////////////////////////////

/// \brief Begin of a standard main
/// \note This macro define 2 variables: int lResult; ArgList ArgList;
/// \see KMS_MAIN2_CONFIG_END KMS_MAIN2_END KMS_MAIN2_RETURN
#define KMS_MAIN2_BEGIN     \
    int lResult = __LINE__; \
    try                     \
    {                       \
        KMS::ArgList lArgList(aCount, aVector)

/// \brief Macro to use after parting arguments
/// \note This macro use lArgList variable the KMS_MAIN2_BEGIN macro defines
/// \see KMS_MAIN2_BEGIN
#define KMS_MAIN2_CONFIG_END                                 \
        if (0 != lArgList.GetCount())                        \
        {                                                    \
            std::cout << Console::Color::YELLOW;             \
            {                                                \
                std::cout << "Unused arguments:";            \
                lArgList.Display(std::cout);                 \
            }                                                \
            std::cout << Console::Color::WHITE << std::endl; \
        }

/// \brief End of a standard main
/// \note This macro use lResult variable the KMS_MAIN2_BEGIN macro defines
/// \see KMS_MAIN2_BEGIN KMS_MAIN2_RETURN
#define KMS_MAIN2_END \
    }                 \
    KMS_CATCH_RESULT(lResult)

/// \brief Return lResult
/// \note This macro use lResult variable the KMS_MAIN2_BEGIN macro defines
/// \see KMS_MAIN2_BEGIN KMS_MAIN2_END
#define KMS_MAIN2_RETURN \
    return lResult
