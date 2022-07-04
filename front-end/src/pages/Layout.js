
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      front-end/src/pages/Layout.js

import { useEffect, useState } from "react";
import { Outlet, Link } from "react-router-dom"

const GetBackEndVersion = () =>
{
    return fetch( 'http://127.0.0.1/GetVersion' )
        .then( ( aResponse ) => { return aResponse.json() } )
        .then( ( aData ) => { return aData.Version } )
        .catch( ( aError ) => { return 'ERROR' } )
}

const Layout = () =>
{
    const [ sBackEndVersion, SetBackEndVersion ] = useState( '' )

    useEffect( () =>
    {
        GetBackEndVersion()
            .then( ( aVersion ) => { SetBackEndVersion( aVersion ); } )
    })

    return (
        <>
            <nav>
                <ul>
                    <li><Link to="/"     >Home </Link></li>
                    <li><Link to="/Test0">Test0</Link></li>
                </ul>
            </nav>

            <Outlet />

            <footer>
                <address>
                    Copyright &copy; 2022 KMS -
                    Back-end { sBackEndVersion } -
                    Front-end 0.0.0
                </address>
            </footer>
        </> )
};

export default Layout;
