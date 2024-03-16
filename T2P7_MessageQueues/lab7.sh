#!/bin/bash
g++ -c lab7_1.cpp
g++ -o lab1 lab7_1.o -lpthread
rm lab7_1.o
g++ -c lab7_2.cpp
g++ -o lab2 lab7_2.o -lpthread
rm lab7_2.o