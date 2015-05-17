#!/bin/bash
grep "^nelem" best.out > nelem.txt
grep "^nvert" best.out > nvert.txt
grep "^heap " best.out > heap.txt
head -n 14 nelem.txt > grow_nelem.txt
head -n 14 nvert.txt > grow_nvert.txt
head -n 15 heap.txt > grow_heap.txt
tail -n 28 nelem.txt > scale_nelem.txt
tail -n 28 nvert.txt > scale_nvert.txt
tail -n 28 heap.txt > scale_heap.txt
awk 'NR%2 == 1' scale_nelem.txt > scl_ref_nelem.txt
awk 'NR%2 == 1' scale_nvert.txt > scl_ref_nvert.txt
awk 'NR%2 == 1' scale_heap.txt > scl_ref_heap.txt
awk 'NR%2 == 0' scale_heap.txt > scl_bal_heap.txt
cut -d ' ' -f 7 scl_ref_nelem.txt > scl_ref_nelem_total.txt
cut -d ' ' -f 11 scl_ref_nelem.txt > scl_ref_nelem_imb.txt
cut -d ' ' -f 11 scl_ref_nvert.txt > scl_ref_nvert_imb.txt
cut -d ' ' -f 3 scl_bal_heap.txt > scl_bal_heap_max.txt
cut -d ' ' -f 9 scl_bal_heap.txt > scl_bal_heap_avg.txt
grep "^balance, comm_size" best.out | cut -d ' ' -f 3 > scl_bal_cs.txt
grep "^refine, comm_size" best.out | cut -d ' ' -f 3 | tail -n 14 > scl_ref_cs.txt
paste scl_ref_cs.txt scl_ref_nelem_imb.txt scl_ref_nvert_imb.txt > scl_ref_imb.dat
paste scl_bal_cs.txt scl_bal_heap_max.txt scl_bal_heap_avg.txt > scl_bal_heap.dat
gnuplot < scl_ref_imb.plt
gnuplot < scl_bal_heap.plt
