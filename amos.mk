CC = mpicc
CFLAGS = -O2 -qstrict -g
MID_CC = gcc
MID_CFLAGS = -std=c99 -O2 -g -Wall -pedantic
MID_LIBS = -lm
GUI = none
BACK = socket
SOCKET = posix
USE_MPI = yes
TARGETS = relay full_test
