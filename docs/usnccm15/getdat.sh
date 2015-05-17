#!/bin/bash
grep "^nelem" best.out > nelem.txt
grep "^nvert" best.out > nvert.txt
grep "^heap " best.out > heap.txt
grep "^refine_subgroup " best.out | cut -d ' ' -f 3  > ref_time.txt
grep "^balance_subgroup " best.out | cut -d ' ' -f 3  > bal_time.txt
grep "^nneigh" best.out > neigh.txt
head -n 14 nelem.txt > grow_nelem.txt
head -n 14 nvert.txt > grow_nvert.txt
head -n 15 heap.txt > grow_heap.txt
head -n 14 ref_time.txt > grow_ref_time.txt
tail -n 28 nelem.txt > scale_nelem.txt
tail -n 28 nvert.txt > scale_nvert.txt
tail -n 28 heap.txt > scale_heap.txt
tail -n 14 ref_time.txt > scl_ref_time.txt
tail -n 14 bal_time.txt > scl_bal_time.txt
tail -n 28 neigh.txt > scale_neigh.txt
awk 'NR%2 == 1' scale_nelem.txt > scl_ref_nelem.txt
awk 'NR%2 == 1' scale_nvert.txt > scl_ref_nvert.txt
awk 'NR%2 == 1' scale_heap.txt > scl_ref_heap.txt
awk 'NR%2 == 0' scale_heap.txt > scl_bal_heap.txt
awk 'NR%2 == 1' scale_neigh.txt > scl_ref_neigh.txt
awk 'NR%2 == 0' scale_neigh.txt > scl_bal_neigh.txt
cut -d ' ' -f 7 scl_ref_nelem.txt > scl_ref_nelem_total.txt
cut -d ' ' -f 11 scl_ref_nelem.txt > scl_ref_nelem_imb.txt
cut -d ' ' -f 11 scl_ref_nvert.txt > scl_ref_nvert_imb.txt
cut -d ' ' -f 3 scl_bal_heap.txt > scl_bal_heap_max.txt
cut -d ' ' -f 9 scl_bal_heap.txt > scl_bal_heap_avg.txt
cut -d ' ' -f 3 scl_ref_neigh.txt > scl_ref_neigh_max.txt
cut -d ' ' -f 9 scl_ref_neigh.txt > scl_ref_neigh_avg.txt
cut -d ' ' -f 3 scl_bal_neigh.txt > scl_bal_neigh_max.txt
cut -d ' ' -f 9 scl_bal_neigh.txt > scl_bal_neigh_avg.txt
grep "^balance, comm_size" best.out | cut -d ' ' -f 3 > scl_bal_cs.txt
grep "^refine, comm_size" best.out | cut -d ' ' -f 3 | tail -n 14 > scl_ref_cs.txt
paste scl_ref_cs.txt scl_ref_nelem_total.txt > scl_ref_nelem_total.dat
paste scl_ref_cs.txt scl_ref_nelem_imb.txt scl_ref_nvert_imb.txt > scl_ref_imb.dat
paste scl_bal_cs.txt scl_bal_heap_max.txt scl_bal_heap_avg.txt > scl_bal_heap.dat
paste scl_ref_cs.txt scl_ref_time.txt > scl_ref_time.dat
paste scl_bal_cs.txt scl_bal_time.txt > scl_bal_time.dat
paste scl_ref_cs.txt scl_ref_neigh_max.txt scl_ref_neigh_avg.txt > scl_ref_neigh.dat
paste scl_bal_cs.txt scl_bal_neigh_max.txt scl_bal_neigh_avg.txt > scl_bal_neigh.dat
gnuplot < scl_ref_nelem_total.plt
gnuplot < scl_ref_imb.plt
gnuplot < scl_bal_heap.plt
gnuplot < scl_time.plt
gnuplot < scl_ref_neigh.plt
gnuplot < scl_bal_neigh.plt
touch DATA
