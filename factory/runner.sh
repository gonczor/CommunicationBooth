#!/bin/bash

# echo "Starting factory..."
# ./factory.exe& 

echo "" > clients.txt
for i in {1..5}
do
	./client.exe&
	PID=$!
	echo $PID >> clients.txt
   	echo "Started process with ID: " $PID
done

echo "Starting dispatcher..."
./dispatcher.exe&


