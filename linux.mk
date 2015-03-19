#options are "cocoa", "w32", and "gtk"
CC = gcc
CFLAGS = -std=c89 -O2 -g -Wall
LDLIBS = -lm
GUI = gtk
BACK = direct
TARGETS = test cad_test quality
