
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/src/components/Footer.js

// Properties
//  Author
//  Version  Front-end version
//  Year

import React, { useEffect, useState } from 'react'

import Styles from './Footer.css'

const Footer = ( aProps ) =>
{
    const [ sBackEndVersion, SetBackEndVersion ] = useState( 'LOADING' )

    const OnData = ( aData ) =>
    {
        SetBackEndVersion( aData.Version );
    }

    const OnError = () =>
    {
        SetBackEndVersion( 'ERROR' )
    }

    // API /Version/GetData
    //  Output
    //      Version  The version number (string)

    useEffect( () =>
    {
        fetch( '/Version/GetData' )
            .then( ( aResponse ) => { return aResponse.json() } )
            .then( OnData )
            .catch( OnError )
    })

    return (
        <footer className = { `${Styles.Footer}` } >
            <address>
                Copyright &copy; { aProps.Year } { aProps.Author } - Back-end { sBackEndVersion } - Front-End { aProps.Version }
            </address>
        </footer> )
}

export { Footer }
