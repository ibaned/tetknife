#options are "cocoa", "w32", and "gtk"
CC = gcc
CFLAGS = -std=c89 -O2 -g -Wall -Wpedantic -Wno-unknown-pragmas -Werror -fno-omit-frame-pointer
LDFLAGS =
LDLIBS = -lm
GUI = gtk
BACK = direct
TARGETS = test cad_test quality myperf
