#!/bin/bash

gcc -o factory.exe factory.c essentials.c
gcc -o dispatcher.exe dispatcher.c essentials.c
gcc -o client.exe client.c essentials.c
