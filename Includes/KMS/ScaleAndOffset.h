
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2022-2026 KMS
/// \file      Includes/KMS/ScaleAndOffset.h

#pragma once

namespace KMS
{

    /// \brief Class to apply scale and offset to `double` values
    /// \note This is what, here at KMS, we call a value class.
    ///       - copy allowed
    ///       - limited size
    ///       - no virtual method
    class ScaleAndOffset final
    {

    public:

        /// \brief Contructor
        /// \param aScale  Scale factor
        /// \param aOffset Offset
        ScaleAndOffset(double aScale, double aOffset = 0.0);

        /// \brief Rever scale and offset
        /// \param aIn Input
        /// \return Converted value
        double Revert(double aIn) const;

        /// \brief Apply scale and offset
        /// \param aIn Input
        /// \return The scaled value
        double Scale (double aIn) const;

    private:

        double mOffset;
        double mScale;

    };

}
