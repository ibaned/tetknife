CC = gcc
CFLAGS = -std=c99 -O2 -g -Wall -Wpedantic
LDFLAGS =
LDLIBS = -lm -lWs2_32
GUI = w32
BACK = socket
SOCKET = w32
USE_MPI = no
TARGETS = viewer full_test
