
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2022-2026 KMS
/// \file      Includes/KMS/RefCount.h

#pragma once

namespace KMS
{

    /// \brief Base class implementing reference counting
    class RefCount
    {

    public:

        /// \brief Decrement the reference counter
        void DecRefCount();

        /// \brief Increment the reference counter
        void IncRefCount();

    protected:

        /// \brief Protected constructor
        RefCount();

        /// \brief Protected destructor
        virtual ~RefCount();

    private:

        unsigned int mRefCount;

    };

}
