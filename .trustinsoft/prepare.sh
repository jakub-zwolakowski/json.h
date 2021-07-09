#!/bin/bash

apt-get update
apt-get -y install cmake

# Build tests (create "generated.h")
cmake test
make

# Make tests
./json_test

# Regenerate TrustInSoft config
./.trustinsoft/regenerate.py