#!/bin/bash
g++ -c lab4_1.cpp
g++ -o lab_child lab4_1.o
rm lab4_1.o
g++ -c lab4_2.cpp
g++ -o lab lab4_2.o -lpthread
rm lab4_2.o