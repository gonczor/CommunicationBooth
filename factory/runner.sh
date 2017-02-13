#!/bin/bash

echo "Starting dispatcher..."
./dispatcher.exe&
DISPATCHER_PID=$!
echo "Dispatcher pid: $DISPATCHER_PID"

echo "Starting factory..."
./factory.exe $DISPATCHER_PID > factory.log&

> clients.txt
for i in {1..1}
do
	./client.exe 1 $DISPATCHER_PID > client$i.log&
	PID=$!
	echo "Started: $PID"
	echo $PID >> clients.txt
done
echo "Sending USR2 to $DISPATCHER_PID"
kill -USR2 $DISPATCHER_PID
