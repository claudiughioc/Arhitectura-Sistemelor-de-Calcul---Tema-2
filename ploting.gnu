set grid
set title 'GNU PLOT'
set xlabel 'Dimensiune matrice (LDAxN)'
set ylabel 'Timp (s)'
set xrange [100000000:*]

set yrange [0:*]



plot    'Hand' using 1:2\
	 title 'Implementare de mana' with linespoints 1 , \
        'Optim' using 1:2\
         title 'Implementare optimizata' with linespoints 2,\
        'Blas' using 1:2\
         title 'Implementare BLAS' with linespoints 3

set size 1.0, 0.6
set terminal postscript portrait enhanced mono dashed lw 1 "Helvetica" 14 
set output "MyTestGcc-N.ps"
replot
set terminal x11
set size 1,1

pause -1 'Press <Return> to stop'
