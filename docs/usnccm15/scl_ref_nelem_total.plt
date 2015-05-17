set terminal svg size 400,262 fname 'Verdana' fsize 10
set output 'scl_ref_nelem_total.svg'
set logscale x 2
set logscale y
set xlabel "procs"
set ylabel "elements"
unset key
plot 'scl_ref_nelem_total.dat' using 1:2 with linespoints

