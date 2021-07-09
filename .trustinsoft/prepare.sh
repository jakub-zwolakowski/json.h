#!/bin/bash

apt-get update
apt-get -y install cmake
apt-get -y install python3.8

# Build tests (create "generated.h")
cmake test
#make

# Make tests
#./json_test

python --version
python3 --version
python3.6 --version
python3.8 --version

# Regenerate TrustInSoft config
./.trustinsoft/regenerate.py