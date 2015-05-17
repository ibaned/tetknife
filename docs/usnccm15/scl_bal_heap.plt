set terminal svg size 350,262 fname 'Verdana' fsize 10
set output 'scl_bal_heap.svg'
set logscale x 2
set xlabel "procs"
set ylabel "bytes"
set yrange [0:1e8]
plot 'scl_bal_heap.dat' using 1:2 title "max heap" with linespoints, \
     'scl_bal_heap.dat' using 1:3 title "avg heap" with linespoints, \
     11.6e6 title "initial heap" with line

