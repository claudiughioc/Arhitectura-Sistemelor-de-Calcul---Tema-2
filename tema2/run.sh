#!/bin/bash
#
# Author: Heri
#
# Script de submitere a job-urilor pe fiecare coda, folosind compilatoare diferite
#

mprun.sh --job-name MyTestGcc-O --queue ibm-opteron.q \
	--modules "compilers/gcc-4.6.0" \
	--script exec_script.sh --show-qsub --show-script --batch-job
mprun.sh --job-name MyTestGcc-N --queue ibm-nehalem.q \
	--modules "compilers/gcc-4.6.0" \
	--script exec_script.sh --show-qsub --show-script --batch-job
mprun.sh --job-name MpiTestGcc-Q --queue ibm-quad.q \
	--modules "compilers/gcc-4.6.0" \
	--script exec_script.sh --show-qsub --show-script --batch-job

