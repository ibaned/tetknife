set terminal png size 500,300
set output 'scl_ref_nelem_total.png'
set logscale x 2
set logscale y
set xlabel "procs"
set ylabel "elements"
unset key
plot 'scl_ref_nelem_total.dat' using 1:2 with linespoints

