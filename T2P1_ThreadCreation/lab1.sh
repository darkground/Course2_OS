#!/bin/bash
g++ -c lab1.cpp
g++ -o lab lab1.o -lpthread
rm lab1.o