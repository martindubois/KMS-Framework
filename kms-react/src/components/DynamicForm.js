
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/src/components/DynamicForm.js

// Properties
//  Id
//  Name

// API /{Name}/GetMetaData
//  Output
//      Fields
//          Index
//          Label
//          Name
//          ReadOnly

// API /{Name}/GetData
//  Input
//      Id
//  Output
//      Data

// API /{Name}/SetData
//  Input
//      Id
//      { Name }

import React, { useEffect, useState } from 'react'

import { DynamicField } from './DynamicField'

const DynamicForm = ( aProps ) =>
{
    const [ sData   , SetData    ] = useState( [] )
    const [ sFields , SetFields  ] = useState( [] )
    const [ sMessage, SetMessage ] = useState( 'Loading...' )

    const OnData = ( aData ) =>
    {
        SetData( aData )
        SetMessage( '' )
    }

    const OnError = () => { SetMessage( 'ERROR' ); }

    const OnLoad  = () =>
    {
        fetch( '/' + aProps.Name + '/GetMetaData' )
            .then( ( aResponse ) => { return aResponse.json() } )
            .then( OnMetaData )
            .catch( OnError )
    }

    const OnMetaData = ( aData ) =>
    {
        SetFields( aData.Fields )
        SetMessage( '' )

        // TODO Pass the Id
        fetch( '/' + aProps.Name + '/GetData' )
            .then( ( aResponse ) => { return aResponse.json() } )
            .then( OnData )
            .catch( OnError )
    }

    const RenderField = ( aField ) =>
    {
        let lData

        if ( 0 == sData.length )
        {
            lData = aField.Default
        }
        else
        {
            lData = sData[ aField.Index ]
        }

        return <DynamicField Data     = { lData }
                             Label    = { aField.Label }
                             Name     = { aField.Name  }
                             ReadOnly = { aField.ReadOnly } />
    }

    useEffect( OnLoad )

    if ( '' != sMessage )
    {
        return <p> { sMessage } </p>
    }

    return (
        <form action = { "/" + aProps.Name + "/SetData" }
              method = "post">
            { sFields.map( RenderField ) }
            <button type = "submit" > Apply </button>
        </form> )
}

export { DynamicForm }
