
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/src/components/DynamicField.js

// TODO Add a Required properties

// Properties
//  Data
//  Label
//  Name
//  ReadOnly

import React, { useState } from 'react'

const DynamicField = ( aProps ) =>
{
    const [ sData, SetData ] = useState( aProps.Data )

    const OnChange = ( aEvent ) => { SetData( aEvent.target.value ) }

    if ( aProps.ReadOnly )
    {
        return (
            <>
                <input name     = { aProps.Name }
                       onchange = { OnChange }
                       readonly
                       type     = "text"
                       value    = { aProps.Data } />
            </> )
    }

    return (
        <>
            <input name     = { aProps.Name }
                   onchange = { OnChange }
                   type     = "text"
                   value    = { aProps.Data } />
        </> )
}

export { DynamicField }
