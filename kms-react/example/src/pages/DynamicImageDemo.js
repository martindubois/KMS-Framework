
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/example/src/pages/DynamicImageDemo.js

import React from 'react'

import { DynamicImage } from 'kms-react'

const DynamicImageDemo = () =>
{
    return (
        <>
            <h1>DynamicImage</h1>
            <DynamicImage Alt       = "Demo"
                          Height    = "512"
                          Name      = "Demo"
                          Period_ms = "1000"
                          Width     = "512" />
        </>
    )
}

export default DynamicImageDemo
