
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      front-end/src/pages/Layout.js

import { Link, Outlet } from "react-router-dom"

import { Footer } from 'kms-react'

const Layout = () =>
{
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

            <Footer Author  = "KMS"
                    Version = "0.0.0"
                    Year    = "2022" />
        </> )
};

export default Layout;
