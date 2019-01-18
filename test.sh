#!/bin/bash
FILES=tests/*
for filename in $FILES; do
    echo $filename
    echo
    sh do.sh $filename
    echo
    echo
done