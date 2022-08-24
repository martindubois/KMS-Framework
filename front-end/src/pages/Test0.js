
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      front-end/src/pages/Test0.js

import { useEffect, useState } from "react";

const Home = () =>
{
    const [ sState, SetState ] = useState('PENDING')

    const OnData = ( aData ) =>
    {
        SetState( 'PASSED' )
    }

    const OnError = ( aError ) =>
    {
        SetState( 'FAILED' )
    }

    const OnLoad = () =>
    {
        fetch( 'http://127.0.0.1/RunTest0' )
            .then( ( aResponse ) => { return aResponse.json() } )
            .then( OnData )
            .catch( OnError )
    }

    useEffect( OnLoad, [] )

    return (
        <>
            <h1>Test0</h1>
            <h2> { sState } </h2>
            <p>The browser will close automaticaly.</p>
        </> )
}

export default Home
