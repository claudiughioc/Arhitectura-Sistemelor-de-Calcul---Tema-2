#!/bin/bash

[[ -z $COMPILER ]]

if [[ $COMPILER="gcc" ]] && COMPILER="gcc"; then
   ./hello_gcc 0
   ./hello_gcc_flags 1
fi

if [[ $COMPILER="suncc" ]] && COMPILER="suncc"; then
   ./hello_suncc 0
   ./hello_suncc_flags 1
fi

if [[ $COMPILER="icc" ]] && COMPILER="icc"; then 
   ./hello_icc 0
   ./hello_icc_flags 1
fi

