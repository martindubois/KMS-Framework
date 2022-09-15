
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      front-end/src/pages/Home.js

import { Link } from "react-router-dom"

const Home = () =>
{
    return (
        <>
            <h1>Home</h1>
            <table>
                <thead>
                    <tr>
                        <td><u>Name</u></td>
                        <td><u>Description</u></td>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td><Link to = "/" >Home</Link></td>
                        <td>This page</td>
                    </tr>
                    <tr>
                        <td><Link to = "ModbusTool" >ModbusTool</Link></td>
                        <td>GUI of the Modbus tool (KMS-ModbusTool)</td>
                    </tr>
                    <tr>
                        <td><Link to = "TestDynamicForm" >Test DynamicForm</Link></td>
                        <td>Page used for test (KMS-B-Test)</td>
                    </tr>
                    <tr>
                        <td><Link to = "TestDynamicImage" >Test DynamicImage</Link></td>
                        <td>Page used for test (KMS-B-Test)</td>
                    </tr>
                </tbody>
            </table>
        </> )
}

export default Home;
