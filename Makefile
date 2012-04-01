#
# Author: Heri
# 
# CC=gcc|suncc|icc
#
GCC=gcc
SCC=suncc
ICC=icc

CFLAGS_gcc=-malign-double -mfpmath=sse -fprefetch-loop-arrays -fwhole-program -fipa-matrix-reorg -freorder-blocks-and-partition -O5
CFLAGS_sun=-xarch=sse3 -xarch=sse3a -fast -O -xprefetch  -xtarget=generic 
CFLAGS_icc=-xhost  -opt-streaming-stores always -ipo

CFLAGS_suncc="-I/opt/libs/blas/atlas-9.11/Atlas-9.11_sun/include/"
LDFLAGS_suncc="-L/opt/libs/blas/atlas-9.11/Atlas-9.11_sun/lib/" -Bstatic -lcblas -latlas -Bdynamic
CFLAGS_icc=""


compile: compilegnu compilesun compileintel

compilegnu:
	$(GCC) -o hello_$(GCC) main.c -static -lcblas -latlas
	$(GCC) -o hello_$(GCC)_flags $(CFLAGS_gcc) main.c -static -lcblas -latlas
compilesun:
	$(SCC) -o hello_$(SCC) $(CFLAGS_suncc) main.c $(LDFLAGS_suncc)
	$(SCC) -o hello_$(SCC)_flags $(CFLAGS_suncc) $(CFLAGS_sun) main.c $(LDFLAGS_suncc)
compileintel:
	$(ICC) -o hello_$(ICC) main.c -static -lcblas -latlas
	$(ICC) -o hello_$(ICC)_flags $(CFLAGS_icc) main.c -static -lcblas -latlas
run:clean
	./hello_$(CC)
clean:
	rm -Rf hello_* out/*
