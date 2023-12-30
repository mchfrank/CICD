#!/bin/bash

if [ -f "output.txt" ];
then
    if [ -s "output.txt" ];
    then
        exit 1
    else
        echo "No style errors were found."
        echo "Code is clang-formatted."
    fi
else
    echo "output.txt file doesn't exist."
    echo "Run the code style check to generate the file."
    exit 1
fi

