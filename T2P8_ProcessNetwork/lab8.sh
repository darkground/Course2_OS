#!/bin/bash
g++ -c lab8_server.cpp
g++ -o lab_server lab8_server.o -lpthread
rm lab8_server.o
g++ -c lab8_client.cpp
g++ -o lab_client lab8_client.o -lpthread
rm lab8_client.o