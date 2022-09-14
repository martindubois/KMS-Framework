
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/example/src/App.js

import React from 'react'

import { BrowserRouter, Route, Routes } from 'react-router-dom'

import 'kms-react/dist/index.css'

import './App.css'

import DynamicFormDemo  from './pages/DynamicFormDemo'
import DynamicImageDemo from './pages/DynamicImageDemo'
import GroupDemo        from './pages/GroupDemo'
import Home             from './pages/Home'
import Layout           from './pages/Layout'
import NoPage           from './pages/NoPage'
 
const App = () =>
{
    return (
        <BrowserRouter>
            <Routes>
                <Route path = "/" element = { <Layout /> }>
                    <Route element = { <Home /> } index />
                    <Route element = { <DynamicFormDemo  /> } path = "DynamicForm"  />
                    <Route element = { <DynamicImageDemo /> } path = "DynamicImage" />
                    <Route element = { <GroupDemo        /> } path = "Group"        />
                    <Route element = { <NoPage /> } path = "*" />
                </Route>
            </Routes>
        </BrowserRouter> )
}

export default App
