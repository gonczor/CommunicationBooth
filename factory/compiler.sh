#!/bin/bash

gcc -o factory.exe factory.c booth.c
gcc -o dispatcher.exe dispatcher.c booth.c
gcc -o client.exe client.c booth.c
