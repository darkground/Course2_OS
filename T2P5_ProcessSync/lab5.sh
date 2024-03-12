#!/bin/bash
g++ -c lab5_1.cpp
g++ -o lab1 lab5_1.o -lpthread
rm lab5_1.o
g++ -c lab5_2.cpp
g++ -o lab2 lab5_2.o -lpthread
rm lab5_2.o