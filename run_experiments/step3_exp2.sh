#!/bin/bash

( cd ./build ; make all )
./build/experiments/step3_exp2/kvstore_step3_exp2
rm -rf Step3Experiment2_*