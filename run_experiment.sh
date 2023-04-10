#!/bin/bash

rm -rf Step1Experiment_*
( cd ./build ; make all )
./build/experiments/step1_exp1/kvstore_step1_exp1
rm -rf Step1Experiment_*