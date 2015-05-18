set terminal png size 500,300
set output 'scl_ref_imb.png'
set logscale x 2
set xlabel "procs"
set ylabel "max / avg"
set yrange [1:1.3]
plot 'scl_ref_imb.dat' using 1:2 title "element imbalance" with linespoints, \
     'scl_ref_imb.dat' using 1:3 title "vertex imbalance" with linespoints
