#!/bin/bash
gcc -Wall -g -o histogram_test histogram_test.c &>log
valgrind --tool=memcheck ./histogram_test
