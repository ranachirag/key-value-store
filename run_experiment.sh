#!/bin/bash

( cd ./build ; make all )
./build/experiments/kvstore_experiments
rm -rf Step1Experiment_*