#!/bin/bash

dir=$1
ext=$2
days=$3
archive_name=$4

files=$(find "$dir" -type f -name "*.$ext" -mtime -"$days")

if [ -z "$files" ]; then #files puste 
  echo "No files found"
  exit 0
fi

tar -cvf "$archive_name" $files

