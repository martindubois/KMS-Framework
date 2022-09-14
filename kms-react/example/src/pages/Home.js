
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/example/src/pages/Home.js

import React from 'react'

import { Link } from 'react-router-dom'

import { Styles } from 'kms-react'

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
                        <td><Link className = { `${Styles.KMS}` } to = "/" >Home</Link></td>
                        <td>This page</td>
                    </tr>
                    <tr>
                        <td><Link className = { `${Styles.KMS}` } to = "/DynamicForm" >DynamicForm</Link></td>
                        <td>DynamicForm</td>
                    </tr>
                    <tr>
                        <td><Link className = { `${Styles.KMS}` } to = "/DynamicImage" >DynamicImage</Link></td>
                        <td>DynamicImage</td>
                    </tr>
                    <tr>
                        <td><Link className = { `${Styles.KMS}` } to = "/Group" >Group</Link></td>
                        <td>Group</td>
                    </tr>
                </tbody>
            </table>
        </> )
}

export default Home;
