
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/src/components/DynamicImage.js

// Properties
//  Alt        See <img>
//  Height     See <img>
//  Name
//  Period_ms
//  Width      See <img>

// API /{Name}/GetData
//  Output
//      Serial  The serial number of the image

import React, { useEffect, useState } from 'react'

const DynamicImage = ( aProps ) =>
{
    const [ sMessage, SetMessage ] = useState( 'Loading...' )
    const [ sSerial , SetSerial  ] = useState( '' )

    const OnTick = () =>
    {
        fetch( '/' + aProps.Name + '/GetData' )
            .then( ( aResponse ) => { return aResponse.json() } )
            .then( OnData )
            .catch( OnError )
    }

    const OnData = ( aData ) =>
    {
        SetMessage( '' )
        SetSerial( aData.Serial )

        setTimeout( OnTick, aProps.Period_ms )
    }

    const OnError = () => { SetMessage( 'ERROR' ) }
    const OnLoad  = () => { setTimeout( OnTick, aProps.Period_ms ) }

    useEffect( OnLoad )

    let lSrc = "/" + aProps.Name + "/" + sSerial + ".bmp"

    let lAlt
    let lLoading

    if ( '' == sMessage )
    {
        lAlt     = aProps.Alt
        lLoading = 'eager'
    }
    else
    {
        lAlt     = sMessage
        lLoading = 'lazy'
    }

    return <img alt     = { lAlt }
                height  = { aProps.Height }
                loading = { lLoading }
                src     = { lSrc }
                width   = { aProps.Width } />
}

export { DynamicImage }
