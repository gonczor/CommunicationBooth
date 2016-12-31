#!/bin/bash

echo "Starting factory..."
./factory.exe > factory.log&

echo "" > clients.txt
for i in {1..5}
do
	./client.exe > client$!.log&
	PID=$!
	echo $PID >> clients.txt
done

echo "Starting dispatcher..."
./dispatcher.exe > dispatcher.log&


