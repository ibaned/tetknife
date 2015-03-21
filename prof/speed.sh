#!/bin/bash -x
perf record -g -- ./$1
perf script | ./prof/stackcollapse-perf.pl > out.perf-folded
./prof/flamegraph.pl --title=$1 out.perf-folded > $1.svg
