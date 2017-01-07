#!/bin/bash

rm *.log

echo "Starting dispatcher..."
./dispatcher.exe&
PID=$!
echo "Dispatcher pid: $PID"

echo "Starting factory..."
./factory.exe "$PID"&

> clients.txt
for i in {1..1}
do
	./client.exe $PID > client$i.log&
	PID=$!
	echo "Started: $PID"
	echo $PID >> clients.txt
done
