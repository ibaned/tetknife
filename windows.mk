#options are "cocoa", "w32", and "gtk"
CC = gcc
CFLAGS = -std=c89 -O2 -g -Wall -Wpedantic -Wno-unknown-pragmas -Werror
LDFLAGS =
LDLIBS = -lm
GUI = w32
BACK = direct
TARGETS = test cad_test quality myperf
