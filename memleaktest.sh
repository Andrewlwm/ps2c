#!/bin/bash

servpid=$(pidof server)

if [ "$servpid" != "" ]; then
    kill $servpid
    echo "Terminated pid:$servpid name:server"
    ./$0
else
    make clean
    make
    ./server > output.txt & servpid=$!
    for ((i=0;i<1000;i++)); do
        ./client localhost > output.txt
        echo "Mem: $(pmap $servpid | tail -n 1 | awk '/[0-9]K/{print $2}')B"
    done
    kill $servpid
    rm -f output.txt
    make clean
fi
