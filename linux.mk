CC = mpicc
FRONT_CC = gcc
CFLAGS = -std=c99 -O2 -g -Wall -pedantic
LDLIBS = -lm
GUI = gtk
BACK = socket
SOCKET = posix
USE_MPI = yes
TARGETS = viewer full_test
