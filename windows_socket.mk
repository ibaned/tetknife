#options are "cocoa", "w32", and "gtk"
CC = gcc
CFLAGS = -std=c89 -O2 -g -Wall -Wpedantic -fno-omit-frame-pointer
LDLIBS = -lWs2_32
GUI = none
BACK = socket
SOCKET = w32
USE_MPI = no
TARGETS = client_test server_test
