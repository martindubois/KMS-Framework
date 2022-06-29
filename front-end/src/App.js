
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      front-end/src/App.js

import { BrowserRouter, Route, Routes } from 'react-router-dom'

import './App.css';

import Layout from './pages/Layout'
import Home   from './pages/Home'
import NoPage from './pages/NoPage'
import Test0  from './pages/Test0'

function App() {
    return (
        <BrowserRouter>
            <Routes>
                <Route path = "/" element = { <Layout /> }>
                    <Route element = { <Home /> } index />
                    <Route element = { <Test0 /> } path = "Test0" />
                    <Route element = { <NoPage /> } path = "*" />
                </Route>
            </Routes>
        </BrowserRouter>
    );
}

export default App;
