#!/bin/bash

git submodule update --init --recursive --remote
# Compiles users assignment files and our tests file, using . as "include's path"
g++ -DNDEBUG -std=c++17 -Wall -pedantic-errors -Werror -g -o TaskManagerTest TaskManager.cpp Task.cpp Person.cpp -I . ./TechnionMatam/HW3/tests/tests.cpp
python3 ./MatamGenericTester/run_tests.py ./TechnionMatam/HW3/tests/custom_tests.json ./TaskManagerTest