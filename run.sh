#!/bin/bash
#
# Author: Heri
#
# Script de submitere a job-urilor pe fiecare coda, folosind compilatoare diferite
#
. /opt/modules/Modules/3.2.5/init/bash
module load blas/atlas-3.9.51-xeon-gcc4.6.0
module load blas/atlas-9.11_sunstudio12.1
module load blas/atlas-3.9.51-nehalem-gcc4.6.0
module load blas/atlas-3.9.51-opteron-gcc4.6.0
module load compilers/gcc-4.6.0
module load compilers/sunstudio12.1
module load compilers/intel-11.0_083
module list
make clean
make

qdel -f -u constantin.ghioc


mprun.sh --job-name Opteron --queue ibm-opteron.q \
	--modules "compilers/gcc-4.6.0" \
	--script exec_script.sh --show-qsub --show-script --batch-job
mprun.sh --job-name Nehalem --queue ibm-nehalem.q \
	--modules "compilers/gcc-4.6.0" \
	--script exec_script.sh --show-qsub --show-script --batch-job
mprun.sh --job-name Quad --queue ibm-quad.q \
	--modules "compilers/gcc-4.6.0" \
	--script exec_script.sh --show-qsub --show-script --batch-job
