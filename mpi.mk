#options are "cocoa", "w32", and "gtk"
CC = mpicc
CFLAGS = -std=c99 -O2 -g -Weverything -Wpedantic -Werror
GUI = none
BACK = direct
TARGETS = commtest migrtest rib_test
USE_MPI = yes
