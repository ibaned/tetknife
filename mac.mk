CC = mpicc
FRONT_CC = clang
CFLAGS = -O2 -g -Weverything -Wpedantic
GUI = cocoa
BACK = socket
SOCKET = posix
USE_MPI = yes
TARGETS = viewer full_test
