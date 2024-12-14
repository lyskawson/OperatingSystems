#!/bin/bash

export LC_ALL=C #ustawienie lokalizacji na standardowa

current_day=$(date +%u) #pon - 1, nd - 7

if [ "$current_day" -gt 5 ]; then #if jest wieksze od 5
    echo "Hello, have a nice weekend!"
else
    echo "Hello!"   
fi

