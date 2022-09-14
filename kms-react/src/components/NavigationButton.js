
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      kms-react/src/components/NavigationButton.js

import React from 'react'

import Styles from './NavigationButton.css'

const NavigationButton = ( aProps ) =>
{
    return (
        <td className = { `${Styles.Navigation}` } >
            <a className = { `${Styles.Navigation}` }
               href      = { aProps.Link } >
                { aProps.Text  }
            </a>
        </td> )
}

export { NavigationButton }
