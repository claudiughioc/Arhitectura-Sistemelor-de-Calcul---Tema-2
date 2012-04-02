set grid
set title 'Compararea timpilor pentru Opteron'
set xlabel 'Tip Implementare (Simpla, Optimizata, Blas, cu Flag-uri)'
set ylabel 'Timp (milisec)'
set xrange [0:3]

set yrange [0:*]

plot 'DOpteron.o*_gcc' using 1\
		title 'Implementare compilator gcc' with linespoints ls 1, \
	'DOpteron.o*_scc' using 1\
		title 'Implementare compilator suncc' with linespoints ls 2,\
	'DOpteron.o*_icc' using 1\
		title 'Implementare compilator intel' with linespoints ls 3


set size 1.0, 0.6
set terminal postscript portrait enhanced mono dashed lw 1 "Arial" 14 
set output "Ghioc_Claudiu_Opteron.ps"
replot
set terminal x11
set size 1,1
