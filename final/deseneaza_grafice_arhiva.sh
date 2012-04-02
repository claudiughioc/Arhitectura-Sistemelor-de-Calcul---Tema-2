#!/bin/bash

cp arhiva/*.o* data
cd data
./parse.sh
export GNUTERM=bla
gnuplot Nehalem.gnu
gnuplot Opteron.gnu
gnuplot Quad.gnu
rm D*
rm *.o*
mv *.ps ..
cd ..
