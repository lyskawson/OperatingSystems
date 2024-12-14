#!/bin/bash

for item in "$@"; do
    if [ -f "$item" ]; then #jesli plik
        echo "$item: plik istnieje"
    elif [ -d "$item" ]; then #jesli katalog
        if [ "$(ls -A "$item")" ]; then #jesli katalog nie jest pusty
            echo "$item: katalog"
        else
            echo "$item: katalog jest pusty"
        fi
    else
        echo "$item: plik nie istnieje"
    fi
done    