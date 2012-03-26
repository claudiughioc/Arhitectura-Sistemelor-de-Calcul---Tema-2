#!/bin/bash

[[ -z $COMPILER ]] && COMPILER="gcc"

if [[ $COMPILER="gcc" ]]; then
   ./hello_gcc
fi

if [[ $COMPILER="suncc" ]]; then
    ./hello_suncc
fi

if [[ $COMPILER="icc" ]]; then 
    ./hello_icc
fi

