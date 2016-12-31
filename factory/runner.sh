#!/bin/bash

echo "Starting factory..."
./factory.exe > factory.log&

> clients.txt
for i in {1..1}
do
	./client.exe > client$!.log&
	PID=$!
	echo "Started: $PID"
	echo $PID >> clients.txt
done

echo "Starting dispatcher..."
./dispatcher.exe > dispatcher.log&


