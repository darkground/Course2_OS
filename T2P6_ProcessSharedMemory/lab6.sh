#!/bin/bash
g++ -c lab6_1.cpp
g++ -o lab1 lab6_1.o -lpthread
rm lab6_1.o
g++ -c lab6_2.cpp
g++ -o lab2 lab6_2.o -lpthread
rm lab6_2.o