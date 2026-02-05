
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2022-2026 KMS
/// \file      Includes/KMS/Console/Color.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

namespace KMS
{
    namespace Console
    {

        /// \brief Use to change color of the output into a terminal
        enum class Color
        {
            BLUE,
            GREEN,
            RED,
            YELLOW,
            WHITE,

            COLOR_QTY,
        };

    }
}

/// \brief Stream output operator
/// \param aOut The output stream
/// \param aC   The color
std::ostream & operator << (std::ostream& aOut, const KMS::Console::Color& aC);
