
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Value.h
// Status    PROD_READY
// Library   KMS-A

// Value_Ptr
//      Mandatory
//          Constructor( InternalType* aPtr )
//          Or
//              operator ExternalType () const
//              operator const InternalType& () const
//      Optional
//          Or
//              void operator = ( ExternalType* aIn )
//              void operator = ( const InternalType& aIn )
//          bool operator == ( ExternalType aIn ) const
//          Or
//              ExternalType Get() const
//              const InternalType& Get() const
//
// Value
//      Mandatory
//          static DI::Object* Create();
//      Optional
//          Constructor()
//          Or
//              Constructor( ExternalType aIn )
//              Constructor( ExternalType a..., const char* aIn )
//              Constructor( const InternalType& aIn )
//              Constructor( const InternalType& a..., const char* aIn )
//          const InternalType& Get...() const;

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Object.h>

namespace KMS
{
    namespace DI
    {

        class Value : public Object
        {

        public:

            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const = 0;

            // Exception  CONVERT_FORMAT_INVALID
            virtual void Set(const char* aIn) = 0;

        protected:

            Value();

        };

        // Protected
        // //////////////////////////////////////////////////////////////////

        inline Value::Value() {}

    }
}
