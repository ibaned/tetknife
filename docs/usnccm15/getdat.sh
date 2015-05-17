#!/bin/bash
grep "^nelem" best.out > nelem.txt
grep "^nvert" best.out > nvert.txt
NLINES=`wc -l nelem.txt | cut -d ' ' -f 7`
head -n 14 nelem.txt > grow_nelem.txt
head -n 14 nvert.txt > grow_nvert.txt
NLINES=`expr $NLINES - 14`
tail -n $NLINES nelem.txt > scale_nelem.txt
tail -n $NLINES nvert.txt > scale_nvert.txt
awk 'NR%2 == 1' scale_nelem.txt > scl_ref_nelem.txt
awk 'NR%2 == 1' scale_nvert.txt > scl_ref_nvert.txt
cut -d ' ' -f 7 scl_ref_nelem.txt > scl_ref_nelem_total.txt
cut -d ' ' -f 11 scl_ref_nelem.txt > scl_ref_nelem_imb.txt
cut -d ' ' -f 11 scl_ref_nvert.txt > scl_ref_nvert_imb.txt
grep "^balance, comm_size" best.out | cut -d ' ' -f 3 > scl_bal_cs.txt
grep "^refine, comm_size" best.out | cut -d ' ' -f 3 | tail -n 14 > scl_ref_cs.txt
paste scl_ref_cs.txt scl_ref_nelem_imb.txt scl_ref_nvert_imb.txt > scl_ref_imb.dat
gnuplot < scl_ref_imb.plt
