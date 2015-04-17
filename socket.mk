#options are "cocoa", "w32", and "gtk"
CC = clang
CFLAGS = -std=c89 -O2 -g -Weverything -Wpedantic -Werror
GUI = none
BACK = socket
SOCKET = unix
USE_MPI = no
TARGETS = client_test server_test
