#!/bin/bash

cd ..

# Install packages
apt-get update
apt-get -y install cmake
apt-get -y install python3.8

# Build tests (create "generated.h")
cmake test

# Make tests
#make
#./json_test

# Regenerate TrustInSoft config
python3.8 .trustinsoft/regenerate.py