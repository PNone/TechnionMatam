#!/bin/bash

git submodule update --init --recursive --remote
g++ -DNDEBUG -std=c++17 -Wall -pedantic-errors -Werror -g -o MvidiaTest MataMvidia.cpp Matrix.cpp Utilities.cpp ./TechnionMatam/HW2/tests/tests.cpp
python3 ./MatamGenericTester/run_tests.py ./MvidiaTest ./TechnionMatam/HW2/tests/custom_tests.json