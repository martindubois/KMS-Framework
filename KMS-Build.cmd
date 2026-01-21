@echo off

rem This file has been genereted using KMS-Framework

rem ===== Verify =====

if not exist C:\_VC\KMS-Framework\_DocUser\Documentation.html (
    echo "ERROR  C:\_VC\KMS-Framework\_DocUser\Documentation.html  does not exist"
    pause
    exit /B 10
)

if not exist C:\_VC\KMS-Framework\_DocUser\KMS.ReadMe.txt (
    echo "ERROR  C:\_VC\KMS-Framework\_DocUser\KMS.ReadMe.txt  does not exist"
    pause
    exit /B 20
)

if not exist "C:\Program Files (x86)\Inno Setup 6\Compil32.exe" (
    echo "ERROR  C:\Program Files (x86)\Inno Setup 6\Compil32.exe  does not exist"
    pause
    exit /B 30
)

if not exist "C:\Program Files\doxygen\bin\doxygen.exe" (
    echo "ERROR  C:\Program Files\doxygen\bin\doxygen.exe  does not exist"
    pause
    exit /B 40
)

if not exist "C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\amd64\MSBuild.exe" (
    echo "ERROR  C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\amd64\MSBuild.exe  does not exist"
    pause
    exit /B 50
)

rem ===== Pre-Build =====

rem ===== Compile =====

"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\amd64\MSBuild.exe" Solution.sln /target:rebuild /Property:Configuration=Debug /property:Platform=x64
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\amd64\MSBuild.exe" Solution.sln /target:rebuild /Property:Configuration=Debug /property:Platform=x86
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\amd64\MSBuild.exe" Solution.sln /target:rebuild /Property:Configuration=Release /property:Platform=x64
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\amd64\MSBuild.exe" Solution.sln /target:rebuild /Property:Configuration=Release /property:Platform=x86
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\amd64\MSBuild.exe" Solution.sln /target:rebuild /Property:Configuration=Release_Static /property:Platform=x64
"C:\Program Files\Microsoft Visual Studio\2022\Professional\Msbuild\Current\Bin\amd64\MSBuild.exe" Solution.sln /target:rebuild /Property:Configuration=Release_Static /property:Platform=x86
rem ===== Test =====

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-Build.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-Build.exe  does not exist"
    pause
    exit /B 60
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-Build.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-Build.pdb  does not exist"
    pause
    exit /B 70
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-Build.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-Build.exe  does not exist"
    pause
    exit /B 80
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-Build.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-Build.pdb  does not exist"
    pause
    exit /B 90
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-Build.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-Build.exe  does not exist"
    pause
    exit /B 100
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-Build.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-Build.pdb  does not exist"
    pause
    exit /B 110
)

if not exist C:\_VC\KMS-Framework\Release\KMS-Build.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-Build.exe  does not exist"
    pause
    exit /B 120
)

if not exist C:\_VC\KMS-Framework\Release\KMS-Build.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-Build.pdb  does not exist"
    pause
    exit /B 130
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-Build.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-Build.exe  does not exist"
    pause
    exit /B 140
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-Build.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-Build.pdb  does not exist"
    pause
    exit /B 150
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-Build.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-Build.exe  does not exist"
    pause
    exit /B 160
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-Build.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-Build.pdb  does not exist"
    pause
    exit /B 170
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-Import.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-Import.exe  does not exist"
    pause
    exit /B 180
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-Import.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-Import.pdb  does not exist"
    pause
    exit /B 190
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-Import.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-Import.exe  does not exist"
    pause
    exit /B 200
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-Import.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-Import.pdb  does not exist"
    pause
    exit /B 210
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-Import.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-Import.exe  does not exist"
    pause
    exit /B 220
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-Import.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-Import.pdb  does not exist"
    pause
    exit /B 230
)

if not exist C:\_VC\KMS-Framework\Release\KMS-Import.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-Import.exe  does not exist"
    pause
    exit /B 240
)

if not exist C:\_VC\KMS-Framework\Release\KMS-Import.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-Import.pdb  does not exist"
    pause
    exit /B 250
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-Import.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-Import.exe  does not exist"
    pause
    exit /B 260
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-Import.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-Import.pdb  does not exist"
    pause
    exit /B 270
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-Import.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-Import.exe  does not exist"
    pause
    exit /B 280
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-Import.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-Import.pdb  does not exist"
    pause
    exit /B 290
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-Make.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-Make.exe  does not exist"
    pause
    exit /B 300
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-Make.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-Make.pdb  does not exist"
    pause
    exit /B 310
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-Make.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-Make.exe  does not exist"
    pause
    exit /B 320
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-Make.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-Make.pdb  does not exist"
    pause
    exit /B 330
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-Make.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-Make.exe  does not exist"
    pause
    exit /B 340
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-Make.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-Make.pdb  does not exist"
    pause
    exit /B 350
)

if not exist C:\_VC\KMS-Framework\Release\KMS-Make.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-Make.exe  does not exist"
    pause
    exit /B 360
)

if not exist C:\_VC\KMS-Framework\Release\KMS-Make.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-Make.pdb  does not exist"
    pause
    exit /B 370
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-Make.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-Make.exe  does not exist"
    pause
    exit /B 380
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-Make.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-Make.pdb  does not exist"
    pause
    exit /B 390
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-Make.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-Make.exe  does not exist"
    pause
    exit /B 400
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-Make.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-Make.pdb  does not exist"
    pause
    exit /B 410
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-Sync.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-Sync.exe  does not exist"
    pause
    exit /B 420
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-Sync.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-Sync.pdb  does not exist"
    pause
    exit /B 430
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-Sync.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-Sync.exe  does not exist"
    pause
    exit /B 440
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-Sync.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-Sync.pdb  does not exist"
    pause
    exit /B 450
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-Sync.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-Sync.exe  does not exist"
    pause
    exit /B 460
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-Sync.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-Sync.pdb  does not exist"
    pause
    exit /B 470
)

if not exist C:\_VC\KMS-Framework\Release\KMS-Sync.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-Sync.exe  does not exist"
    pause
    exit /B 480
)

if not exist C:\_VC\KMS-Framework\Release\KMS-Sync.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-Sync.pdb  does not exist"
    pause
    exit /B 490
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-Sync.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-Sync.exe  does not exist"
    pause
    exit /B 500
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-Sync.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-Sync.pdb  does not exist"
    pause
    exit /B 510
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-Sync.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-Sync.exe  does not exist"
    pause
    exit /B 520
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-Sync.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-Sync.pdb  does not exist"
    pause
    exit /B 530
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-A.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-A.lib  does not exist"
    pause
    exit /B 540
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-A.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-A.pdb  does not exist"
    pause
    exit /B 550
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-A.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-A.lib  does not exist"
    pause
    exit /B 560
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-A.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-A.pdb  does not exist"
    pause
    exit /B 570
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-A.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-A.lib  does not exist"
    pause
    exit /B 580
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-A.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-A.pdb  does not exist"
    pause
    exit /B 590
)

if not exist C:\_VC\KMS-Framework\Release\KMS-A.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-A.lib  does not exist"
    pause
    exit /B 600
)

if not exist C:\_VC\KMS-Framework\Release\KMS-A.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-A.pdb  does not exist"
    pause
    exit /B 610
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-A.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-A.lib  does not exist"
    pause
    exit /B 620
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-A.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-A.pdb  does not exist"
    pause
    exit /B 630
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-A.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-A.lib  does not exist"
    pause
    exit /B 640
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-A.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-A.pdb  does not exist"
    pause
    exit /B 650
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-B.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-B.lib  does not exist"
    pause
    exit /B 660
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-B.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-B.pdb  does not exist"
    pause
    exit /B 670
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-B.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-B.lib  does not exist"
    pause
    exit /B 680
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-B.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-B.pdb  does not exist"
    pause
    exit /B 690
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-B.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-B.lib  does not exist"
    pause
    exit /B 700
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-B.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-B.pdb  does not exist"
    pause
    exit /B 710
)

if not exist C:\_VC\KMS-Framework\Release\KMS-B.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-B.lib  does not exist"
    pause
    exit /B 720
)

if not exist C:\_VC\KMS-Framework\Release\KMS-B.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-B.pdb  does not exist"
    pause
    exit /B 730
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-B.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-B.lib  does not exist"
    pause
    exit /B 740
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-B.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-B.pdb  does not exist"
    pause
    exit /B 750
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-B.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-B.lib  does not exist"
    pause
    exit /B 760
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-B.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-B.pdb  does not exist"
    pause
    exit /B 770
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-C.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-C.lib  does not exist"
    pause
    exit /B 780
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-C.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-C.pdb  does not exist"
    pause
    exit /B 790
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-C.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-C.lib  does not exist"
    pause
    exit /B 800
)

if not exist C:\_VC\KMS-Framework\Debug\KMS-C.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-C.pdb  does not exist"
    pause
    exit /B 810
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-C.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-C.lib  does not exist"
    pause
    exit /B 820
)

if not exist C:\_VC\KMS-Framework\x64\Release\KMS-C.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-C.pdb  does not exist"
    pause
    exit /B 830
)

if not exist C:\_VC\KMS-Framework\Release\KMS-C.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-C.lib  does not exist"
    pause
    exit /B 840
)

if not exist C:\_VC\KMS-Framework\Release\KMS-C.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-C.pdb  does not exist"
    pause
    exit /B 850
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-C.lib (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-C.lib  does not exist"
    pause
    exit /B 860
)

if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-C.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-C.pdb  does not exist"
    pause
    exit /B 870
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-C.lib (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-C.lib  does not exist"
    pause
    exit /B 880
)

if not exist C:\_VC\KMS-Framework\Release_Static\KMS-C.pdb (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-C.pdb  does not exist"
    pause
    exit /B 890
)

if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-A-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-A-Test.exe  does not exist"
    pause
    exit /B 900
)

"C:\_VC\KMS-Framework\x64\Debug\KMS-A-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Debug\KMS-A-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-A-Test.exe  does not exist"
    pause
    exit /B 910
)

"C:\_VC\KMS-Framework\Debug\KMS-A-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\x64\Release\KMS-A-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-A-Test.exe  does not exist"
    pause
    exit /B 920
)

"C:\_VC\KMS-Framework\x64\Release\KMS-A-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Release\KMS-A-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-A-Test.exe  does not exist"
    pause
    exit /B 930
)

"C:\_VC\KMS-Framework\Release\KMS-A-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-A-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-A-Test.exe  does not exist"
    pause
    exit /B 940
)

"C:\_VC\KMS-Framework\x64\Release_Static\KMS-A-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Release_Static\KMS-A-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-A-Test.exe  does not exist"
    pause
    exit /B 950
)

"C:\_VC\KMS-Framework\Release_Static\KMS-A-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-B-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-B-Test.exe  does not exist"
    pause
    exit /B 960
)

"C:\_VC\KMS-Framework\x64\Debug\KMS-B-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Debug\KMS-B-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-B-Test.exe  does not exist"
    pause
    exit /B 970
)

"C:\_VC\KMS-Framework\Debug\KMS-B-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\x64\Release\KMS-B-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-B-Test.exe  does not exist"
    pause
    exit /B 980
)

"C:\_VC\KMS-Framework\x64\Release\KMS-B-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Release\KMS-B-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-B-Test.exe  does not exist"
    pause
    exit /B 990
)

"C:\_VC\KMS-Framework\Release\KMS-B-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-B-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-B-Test.exe  does not exist"
    pause
    exit /B 1000
)

"C:\_VC\KMS-Framework\x64\Release_Static\KMS-B-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Release_Static\KMS-B-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-B-Test.exe  does not exist"
    pause
    exit /B 1010
)

"C:\_VC\KMS-Framework\Release_Static\KMS-B-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\x64\Debug\KMS-C-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Debug\KMS-C-Test.exe  does not exist"
    pause
    exit /B 1020
)

"C:\_VC\KMS-Framework\x64\Debug\KMS-C-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Debug\KMS-C-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Debug\KMS-C-Test.exe  does not exist"
    pause
    exit /B 1030
)

"C:\_VC\KMS-Framework\Debug\KMS-C-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\x64\Release\KMS-C-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release\KMS-C-Test.exe  does not exist"
    pause
    exit /B 1040
)

"C:\_VC\KMS-Framework\x64\Release\KMS-C-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Release\KMS-C-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release\KMS-C-Test.exe  does not exist"
    pause
    exit /B 1050
)

"C:\_VC\KMS-Framework\Release\KMS-C-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\x64\Release_Static\KMS-C-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\x64\Release_Static\KMS-C-Test.exe  does not exist"
    pause
    exit /B 1060
)

"C:\_VC\KMS-Framework\x64\Release_Static\KMS-C-Test.exe" Groups+=Auto
if not exist C:\_VC\KMS-Framework\Release_Static\KMS-C-Test.exe (
    echo "ERROR  C:\_VC\KMS-Framework\Release_Static\KMS-C-Test.exe  does not exist"
    pause
    exit /B 1070
)

"C:\_VC\KMS-Framework\Release_Static\KMS-C-Test.exe" Groups+=Auto
"C:\Program Files\doxygen\bin\doxygen.exe" DoxyFile_en.txt
rem ===== Package =====

"C:\Program Files (x86)\Inno Setup 6\Compil32.exe" /cc Product_x64.iss
rem ===== Sign =====


rem ===== End =====
echo OK
exit /B 0
