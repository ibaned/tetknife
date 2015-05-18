set terminal png size 500,300
set output 'scl_time.png'
set logscale x 2
set xlabel "procs"
set ylabel "seconds"
set yrange [0:32]
plot 'scl_ref_time.dat' title "refine time" with linespoints, \
     'scl_bal_time.dat' title "balance time" with linespoints

