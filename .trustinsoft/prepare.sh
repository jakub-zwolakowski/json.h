#!/bin/bash

apt-get update
apt-get -y install cmake
apt-get -y install python3.8

# Build tests (create "generated.h")
cmake test
#make

# Make tests
#./json_test

# Regenerate TrustInSoft config
./.trustinsoft/regenerate.py