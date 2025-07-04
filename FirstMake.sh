#!/bin/sh

# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2025 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      FirstMake.sh

echo Executing  FirstMake.sh  ...

# ===== Execution ===========================================================

mkdir Binaries
mkdir Binaries/Debug_x86_64
mkdir Libraries
mkdir Libraries/Debug_x86_64

echo Making KMS-A ...

cd KMS-A
make CONFIG=Debug PROCESSOR=x86_64
cd ..

echo Making KMS-Make ...

cd KMS-Make
make CONFIG=Debug PROCESSOR=x86_64
cd ..

# ===== End =================================================================

echo OK
