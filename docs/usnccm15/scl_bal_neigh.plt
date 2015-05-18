set terminal png size 400,400
set output 'scl_bal_neigh.png'
set title "after balancing"
set logscale x 2
set xlabel "procs"
set ylabel "neighbors"
set yrange [0:40]
plot 'scl_bal_neigh.dat' using 1:2 title "max neighbors" with linespoints, \
     'scl_bal_neigh.dat' using 1:3 title "avg neighbors" with linespoints


