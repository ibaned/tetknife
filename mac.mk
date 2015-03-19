#options are "cocoa", "w32", and "gtk"
CC = clang
CFLAGS = -std=c89 -O2 -g -Weverything -Wpedantic -Werror
GUI = cocoa
BACK = direct
TARGETS = test cad_test quality
