#!/bin/sh

# This file has been genereted using KMS-Framework

# ===== Verify =====

if [ ! -e "/home/mdubois/_VC/KMS-Framework/_DocUser/Documentation.html" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/_DocUser/Documentation.html  does not exist
    read -p "Press Enter to continue..."
    exit 10
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/_DocUser/KMS.ReadMe.txt" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/_DocUser/KMS.ReadMe.txt  does not exist
    read -p "Press Enter to continue..."
    exit 20
fi

# ===== Pre-Build =====

# ===== Compile =====

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries" ] ; then
    mkdir /home/mdubois/_VC/KMS-Framework/Binaries
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries" ] ; then
    mkdir /home/mdubois/_VC/KMS-Framework/Libraries
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64" ] ; then
    mkdir /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64" ] ; then
    mkdir /home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-Build/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-Build/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Build" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Build
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-Import/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-Import/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Import" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Import
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-Make/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-Make/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-A/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-A/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-A.a" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-A.a
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-B/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-B/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-B.a" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-B.a
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-C/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-C/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-C.a" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-C.a
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-A-Test/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-A-Test/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-A-Test" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-A-Test
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-B-Test/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-B-Test/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-B-Test" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-B-Test
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-C-Test/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-C-Test/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-C-Test" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-C-Test
fi

pushd KMS-A
"make" CONFIG=Debug PROCESSOR=x86_64
popd
pushd KMS-B
"make" CONFIG=Debug PROCESSOR=x86_64
popd
pushd KMS-C
"make" CONFIG=Debug PROCESSOR=x86_64
popd
pushd KMS-A-Test
"make" CONFIG=Debug PROCESSOR=x86_64
popd
pushd KMS-B-Test
"make" CONFIG=Debug PROCESSOR=x86_64
popd
pushd KMS-C-Test
"make" CONFIG=Debug PROCESSOR=x86_64
popd
pushd KMS-Build
"make" CONFIG=Debug PROCESSOR=x86_64
popd
pushd KMS-Import
"make" CONFIG=Debug PROCESSOR=x86_64
popd
pushd KMS-Make
"make" CONFIG=Debug PROCESSOR=x86_64
popd
if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries" ] ; then
    mkdir /home/mdubois/_VC/KMS-Framework/Binaries
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries" ] ; then
    mkdir /home/mdubois/_VC/KMS-Framework/Libraries
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64" ] ; then
    mkdir /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64" ] ; then
    mkdir /home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-Build/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-Build/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Build" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Build
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-Import/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-Import/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Import" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Import
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-Make/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-Make/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-A/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-A/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-A.a" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-A.a
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-B/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-B/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-B.a" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-B.a
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-C/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-C/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-C.a" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-C.a
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-A-Test/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-A-Test/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-A-Test" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-A-Test
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-B-Test/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-B-Test/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-B-Test" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-B-Test
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/KMS-C-Test/*.o" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/KMS-C-Test/*.o
fi

if [ -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-C-Test" ] ; then
    rm -f /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-C-Test
fi

pushd KMS-A
"make" CONFIG=Release PROCESSOR=x86_64
popd
pushd KMS-B
"make" CONFIG=Release PROCESSOR=x86_64
popd
pushd KMS-C
"make" CONFIG=Release PROCESSOR=x86_64
popd
pushd KMS-A-Test
"make" CONFIG=Release PROCESSOR=x86_64
popd
pushd KMS-B-Test
"make" CONFIG=Release PROCESSOR=x86_64
popd
pushd KMS-C-Test
"make" CONFIG=Release PROCESSOR=x86_64
popd
pushd KMS-Build
"make" CONFIG=Release PROCESSOR=x86_64
popd
pushd KMS-Import
"make" CONFIG=Release PROCESSOR=x86_64
popd
pushd KMS-Make
"make" CONFIG=Release PROCESSOR=x86_64
popd
# ===== Test =====

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Build" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Build  does not exist
    read -p "Press Enter to continue..."
    exit 30
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Build" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Build  does not exist
    read -p "Press Enter to continue..."
    exit 40
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Import" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Import  does not exist
    read -p "Press Enter to continue..."
    exit 50
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Import" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Import  does not exist
    read -p "Press Enter to continue..."
    exit 60
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Make" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-Make  does not exist
    read -p "Press Enter to continue..."
    exit 70
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Make" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-Make  does not exist
    read -p "Press Enter to continue..."
    exit 80
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-A.a" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-A.a  does not exist
    read -p "Press Enter to continue..."
    exit 90
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-A.a" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-A.a  does not exist
    read -p "Press Enter to continue..."
    exit 100
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-B.a" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-B.a  does not exist
    read -p "Press Enter to continue..."
    exit 110
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-B.a" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-B.a  does not exist
    read -p "Press Enter to continue..."
    exit 120
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-C.a" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Libraries/Debug_x86_64/KMS-C.a  does not exist
    read -p "Press Enter to continue..."
    exit 130
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-C.a" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Libraries/Release_x86_64/KMS-C.a  does not exist
    read -p "Press Enter to continue..."
    exit 140
fi

if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-A-Test" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-A-Test  does not exist
    read -p "Press Enter to continue..."
    exit 150
fi

"/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-A-Test" Groups+=Auto
if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-A-Test" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-A-Test  does not exist
    read -p "Press Enter to continue..."
    exit 160
fi

"/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-A-Test" Groups+=Auto
if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-B-Test" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-B-Test  does not exist
    read -p "Press Enter to continue..."
    exit 170
fi

"/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-B-Test" Groups+=Auto
if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-B-Test" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-B-Test  does not exist
    read -p "Press Enter to continue..."
    exit 180
fi

"/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-B-Test" Groups+=Auto
if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-C-Test" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-C-Test  does not exist
    read -p "Press Enter to continue..."
    exit 190
fi

"/home/mdubois/_VC/KMS-Framework/Binaries/Debug_x86_64/KMS-C-Test" Groups+=Auto
if [ ! -e "/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-C-Test" ] ; then
    echo ERROR  /home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-C-Test  does not exist
    read -p "Press Enter to continue..."
    exit 200
fi

"/home/mdubois/_VC/KMS-Framework/Binaries/Release_x86_64/KMS-C-Test" Groups+=Auto
"doxygen" DoxyFile_en.txt
# ===== Package =====

# ===== Sign =====


# ===== End =====
echo OK
