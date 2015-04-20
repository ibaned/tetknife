CC = mpicc
CFLAGS = -std=c99 -O2 -g -Weverything -Wpedantic -Werror
GUI = cocoa
BACK = socket
SOCKET = posix
USE_MPI = yes
TARGETS = reduce_test xmesh
