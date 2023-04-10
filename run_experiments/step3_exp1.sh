#!/bin/bash

( cd ./build ; make all )
./build/experiments/step3_exp1/kvstore_step3_exp1
rm -rf Step3Experiment1_*