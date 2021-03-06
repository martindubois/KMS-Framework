#!/bin/sh

# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      Scripts/DevComputer_Linux.sh

echo Executing  Scripts/DevComputer_Linux.sh  ...

# ===== Executing ===========================================================

sudo apt install apt-transport-https

sudo apt install curl

sudo apt install gpg

sudo apt install wget

curl -sL https://deb.nodesource.com/setup_18.x -o Temp.sh
sudo bash Temp.sh

wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg
sudo install -o root -g root -m 644 packages.microsoft.gpg /etc/apt/trusted.gpg.d/
sudo sh -c 'echo "deb [arch=amd64,arm64,armhf signed-by=/etc/apt/trusted.gpg.d/packages.microsoft.gpg] https://packages.microsoft.com/repos/code stable main" > /etc/apt/sources.list.d/vscode.list'
rm -f packages.microsoft.gpg

sudo apt update

sudo apt install code

sudo apt install g++

sudo apt install git-gui

sudo apt install nodejs

sudo apt install npm

sudo apt install xutils-dev

# ===== End =================================================================

echo OK
