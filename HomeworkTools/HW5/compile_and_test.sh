#!/bin/bash

git submodule update --init --recursive --remote
export MATAM_TESTER_TRIMR_SPACES=1
export MATAM_TESTER_IGNORE_EMPTY_LINES=1
python3 ./MatamGenericTester/run_tests.py ./TechnionMatam/HW5/tests/custom_tests.json /usr/bin/python3 ./enigma.py