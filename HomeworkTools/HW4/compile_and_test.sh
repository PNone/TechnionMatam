#!/bin/bash

git submodule update --init --recursive --remote
# Compiles users assignment files and our tests file, using . as "include's path"
g++ -DNDEBUG -std=c++17 -Wall -pedantic-errors -Werror -g -o MatamStoryTest *.cpp Events/*.cpp Players/*.cpp -I. -I./Players -I./Events
python3 ./MatamGenericTester/run_tests.py ./TechnionMatam/HW4/tests/custom_tests.json ./MatamStoryTest