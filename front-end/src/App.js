
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      front-end/src/App.js

import { BrowserRouter, Route, Routes } from 'react-router-dom'

import './App.css';

import Layout           from './pages/Layout'
import Home             from './pages/Home'
import ModbustTool      from './pages/ModbusTool'
import NoPage           from './pages/NoPage'
import TestDynamicForm  from './pages/TestDynamicForm'
import TestDynamicImage from './pages/TestDynamicImage'

function App()
{
    return (
        <BrowserRouter>
            <Routes>
                <Route path = "/" element = { <Layout /> }>
                    <Route element = { <Home /> } index />
                    <Route element = { <ModbustTool      /> } path = "ModbusTool"       />
                    <Route element = { <TestDynamicForm  /> } path = "TestDynamicForm"  />
                    <Route element = { <TestDynamicImage /> } path = "TestDynamicImage" />
                    <Route element = { <NoPage /> } path = "*" />
                </Route>
            </Routes>
        </BrowserRouter> )
}

export default App
