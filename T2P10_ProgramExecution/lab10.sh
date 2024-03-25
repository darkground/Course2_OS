#!/bin/bash
g++ -c lab10_1.cpp
g++ -o lab_child lab10_1.o
rm lab10_1.o
g++ -c lab10_2.cpp
g++ -o lab lab10_2.o -lpthread -lev
rm lab10_2.o