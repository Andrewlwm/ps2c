#!/bin/bash

servpid=$(pidof server)
$con=0
if [ "$servpid" != "" ]; then
    kill $servpid
    echo "Terminated pid:$servpid name:server"
    ./$0
else
    make clean
    make
    ./server >/dev/null & servpid=$!
    for ((i=0;i<5000;i++)); do
        ./client localhost > /dev/null
        if !(($i % 25 )) ; then
            echo "Mem: $(pmap $servpid | tail -n 1 | awk '/[0-9]K/{print $2}')B"
            ((con++))
        fi
    done
    kill $servpid
    make clean
    echo $con
fi
