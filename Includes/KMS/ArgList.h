
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2026 KMS
/// \file      Includes/KMS/ArgList.h

#pragma once

// ===== C++ ================================================================
#include <iostream>

namespace KMS
{

    /// \brief Class adding use count to an argument list
    class ArgList final
    {

    public:

        /// \brief Value indicating a invalid use count
        static const unsigned int INVALID_USE_COUNT;

        /// \brief Constructor
        /// \param aCount  The argument count
        /// \param aVector The argument vector
        ArgList(int aCount, const char** aVector);

        /// \brief Destructor
        ~ArgList();

        /// \param aIndex The argument index
        /// \retval nullptr Invalid index
        ///         Other   The argument
        const char* GetArgument(unsigned int aIndex) const;

        /// \retval The number of not removed arguments
        unsigned int GetCount() const;

        /// \param aIndex The argument index
        /// \retval INVALID_USE_COUNT Invalid index
        ///         Other             The use count of the argument
        unsigned int GetUseCount(unsigned int aIndex) const;

        /// \param aOut The output stream
        void Display(std::ostream& aOut);

        /// \parm aIndex The argument index
        void IncUseCount(unsigned int aIndex);

        /// \brief Remove used arguments
        void RemoveUsed();

    private:

        unsigned int GetInternalIndex(unsigned int aIndex) const;

        unsigned int mCount;
        const char** mVector;

        unsigned int* mUseCounts;

    };

}
