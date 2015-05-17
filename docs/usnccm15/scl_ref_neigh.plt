set terminal svg size 350,262 fname 'Verdana' fsize 10
set output 'scl_ref_neigh.svg'
set logscale x 2
set xlabel "procs"
set ylabel "neighbors"
set yrange [0:40]
plot 'scl_ref_neigh.dat' using 1:2 title "max neighbors" with linespoints, \
     'scl_ref_neigh.dat' using 1:3 title "avg neighbors" with linespoints

