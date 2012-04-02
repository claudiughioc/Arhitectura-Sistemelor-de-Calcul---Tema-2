#!/bin/bash

cp out/* data
cd data
./parse.sh
gnuplot Nehalem.gnu
gnuplot Opteron.gnu
gnuplot Quad.gnu
rm D*
rm *.o*
mv *.ps ..
cd ..
