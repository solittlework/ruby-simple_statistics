#!/bin/bash
# valgrind test script for histogram functions
gcc -Wall -g -o histogram_test histogram_test.c &>log
valgrind --tool=memcheck ./histogram_test
