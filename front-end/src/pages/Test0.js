
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      front-end/src/pages/Test0.js

import { useEffect, useState } from "react";

const Test0 = () =>
{
    return fetch( 'http://127.0.0.1/Test0' )
        .then( ( aResponse ) => { return 'PASSED' } )
        .catch( ( aError ) => { return 'FAILED' } )
}

const Home = () =>
{
    const [ sState, SetState ] = useState('PENDING')

    useEffect(() =>
    {
        Test0()
            .then( ( aState ) => { SetState( aState ); } )
    })

    return (
        <>
            <h1>Test0</h1>
            <h2> { sState } </h2>
            <p>The browser will close automaticaly.</p>
        </> )
}

export default Home