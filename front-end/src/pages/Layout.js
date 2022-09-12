
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      front-end/src/pages/Layout.js

import { useEffect, useState } from "react";
import { Link, Outlet } from "react-router-dom"

const Layout = () =>
{
    const [ sBackEndVersion, SetBackEndVersion ] = useState( '' )

    const OnData = ( aData ) =>
    {
        SetBackEndVersion( aData.Version )
    }

    const OnError = ( aError ) =>
    {
        SetBackEndVersion( 'ERROR' )
    }

    const OnLoad = () =>
    {
        fetch( 'http://127.0.0.1/GetVersion' )
            .then( ( aResponse ) => { return aResponse.json() } )
            .then( OnData )
            .catch( OnError )
    }

    useEffect( OnLoad, [] )

    return (
        <>
            <nav>
                <table>
                    <tbody>
                        <tr>
                            <td className = "Nav" ><Link to = "/" >Home</Link></td>
                        </tr>
                    </tbody>
                </table>
            </nav>

            <div className = "Main" >
                <Outlet />
            </div>

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
