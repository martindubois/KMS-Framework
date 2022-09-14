
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/example/src/pages/Layout.js

import React from 'react'

import { Outlet } from 'react-router-dom'

import { Footer, NavigationButton, Styles } from 'kms-react'

const Layout = () =>
{
    return (
        <>
            <nav>
                <table>
                    <tbody>
                        <tr>
                            <NavigationButton Link = "/"             Text = "Home"         />
                            <NavigationButton Link = "/DynamicForm"  Text = "DynamicForm"  />
                            <NavigationButton Link = "/DynamicImage" Text = "DynamicImage" />
                            <NavigationButton Link = "/Group"        Text = "Group"        />
                        </tr>
                    </tbody>
                </table>
            </nav>

            <div className = { `${Styles.Screen}` } >
                <Outlet />
            </div>

            <Footer Author = "KMS" Version = "9.9.9" Year = "2022" />
        </> )
};

export default Layout;
