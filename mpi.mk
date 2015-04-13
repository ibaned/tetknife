#options are "cocoa", "w32", and "gtk"
CC = mpicc
CFLAGS = -std=c89 -O2 -g -Weverything -Wpedantic -Werror
GUI = none
BACK = direct
TARGETS = commtest
USE_MPI = yes
