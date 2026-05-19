
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2026 KMS
/// \file      Includes/KMS/DI2/GUID.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI2/IType.h>

namespace KMS
{
    namespace DI2
    {

        class GUID final : public IType
        {

        public:

            // ===== IType ==================================================
            virtual void Code_ASCII(const void* aData, Output* aOutput) const override;
            virtual void Code_JSON(const void* aData, Output* aOutput) const override;
            virtual void Decode_ASCII(void* aData, Input* aInput) const override;
            virtual void Decode_JSON(void* aData, Input* aInput) const override;

        };

        /// \brief DI2 type description for GUID
        /// \note Only on Windows
        extern const GUID TYPE_GUID;

    }
}
