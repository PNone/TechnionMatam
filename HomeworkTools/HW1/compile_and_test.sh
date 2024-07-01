#!/bin/bash

git submodule update --init --recursive --remote
g++ -DNDEBUG -std=c++17 -Wall -pedantic-errors -Werror -g -o mtm_blockchain *.cpp
python3 ./MatamGenericTester/run_tests.py ./mtm_blockchain ./TechnionMatam/HW1/tests/custom_tests.json