set grid
set title 'Compararea timpilor pentru Nehalem'
set xlabel 'Tip Compilator'
set ylabel 'Timp (milisec)'
set xrange [0:*]

set yrange [0:*]

plot 'DNehalem.o*_gcc' using 1:2\
		title 'Implementare compilator gcc' with linespoints 1 , \
	'DNehalem.o*_scc' using 1:2\
		title 'Implementare compilator suncc' with linespoints 2,\
	'DNehalem.o*_icc' using 1:2\
		title 'Implementare compilator intel' with linespoints 3;


set size 1.0, 0.6
set terminal postscript portrait enhanced mono dashed lw 1 "Arial" 14 
set output "Ghioc_Claudiu_Nehalem.ps"
replot
set terminal x11
set size 1,1

pause -1 'Press <Return> to stop'
