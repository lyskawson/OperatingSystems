#!/bin/bash

if [ "$1" == "-a" ]; then
   file_count=$(ls -1a | wc -l)
else
    file_count=$(ls -1 | wc -l)
fi

echo "Number of files in directory: $file_count"

