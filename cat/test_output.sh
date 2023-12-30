#!/bin/bash

if [ -f "log.txt" ];
then
    if [ -s "log.txt" ];
    then
        exit 1
    else
        echo "No test errors found."
    fi
else
    echo "log.txt file doesn't exist."
    echo "Run automated tests to generate the file."
    exit 1
fi

