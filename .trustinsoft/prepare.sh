#!/bin/bash

# Install packages
apt-get update
apt-get -y install python3.8

# Build tests (create "generated.h")
pushd ..
cmake test
popd

# Make tests
# pushd ..
#make
#./json_test
#popd

# Regenerate TrustInSoft config
python3.8 regenerate.py