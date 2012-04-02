set grid
set title 'Compararea timpilor pentru Nehalem'
set xlabel 'Tip Implementare (Simpla, Optimizata, Blas, cu Flag-uri)'
set ylabel 'Timp (milisec)'
set xrange [0:3]

set yrange [0:*]

plot 'DNehalem.o*_gcc' using 1\
		title 'Implementare compilator gcc' with linespoints  1, \
	'DNehalem.o*_scc' using 1\
		title 'Implementare compilator suncc' with linespoints 2,\
	'DNehalem.o*_icc' using 1\
		title 'Implementare compilator intel' with linespoints 3


set size 1.0, 0.6
set terminal postscript portrait enhanced mono dashed lw 1 "Arial" 14 
set output "Ghioc_Claudiu_Nehalem.ps"
replot
set size 1,1
