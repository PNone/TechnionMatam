#!/bin/bash

git submodule update --init --recursive --remote
g++ -DNDEBUG -std=c++17 -Wall -pedantic-errors -Werror -g -o Mvidia *.cpp
python3 ./MatamGenericTester/run_tests.py ./Mvidia ./TechnionMatam/HW2/tests/custom_tests.json