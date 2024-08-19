#!/bin/bash

git submodule update --init --recursive --remote
python3 ./MatamGenericTester/run_tests.py python3 ./enigma.py ./TechnionMatam/HW5/tests/custom_tests.json