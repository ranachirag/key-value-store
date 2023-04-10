FROM ubuntu:latest

RUN apt-get update

RUN apt-get install -y g++

RUN apt-get install -y git build-essential curl tar zip unzip cmake ninja-build pkg-config

RUN apt-get clean && rm -rf /var/lib/apt/lists/*

ENV VCPKG_FORCE_SYSTEM_BINARIES=1

# git clone --recurse-submodule https://github.com/ranachirag/CSC443-project.git

# cd CSC443-project

# ./build.sh