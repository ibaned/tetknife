CONFIG ?= default

include $(CONFIG).mk

BACK_OBJS = basics.o

ifeq "$(GUI)" "cocoa"
GUI_OBJ += cocoa_main.o
GUI_LIBS = -framework Cocoa
else ifeq "$(GUI)" "gtk"
GTK_FLAGS = $(shell pkg-config --cflags gtk+-2.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-2.0)
GUI_OBJ += gtk_main.o
GUI_LIBS = $(GTK_LIBS)
else ifeq "$(GUI)" "w32"
GUI_OBJ += w32_main.o
GUI_LIBS = -lGid32
endif

ifeq "$(BACK)" "direct"
FRONT_CC = $(CC)
FRONT_OBJS = $(GUI_OBJ) front_back.o $(BACK_OBJS)
FRONT_LIBS = $(GUI_LIBS) $(LDLIBS)
FRONT_LDFLAGS = $(LDFLAGS)
FRONT_CFLAGS = $(CFLAGS)
endif

FRONT_LINK = $(FRONT_CC) $(FRONT_LDFLAGS)
FRONT_COMPILE = $(FRONT_CC) $(FRONT_CFLAGS)
BACK_COMPILE = $(CC) $(CFLAGS)

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

test: test.o $(FRONT_OBJS)
	$(FRONT_LINK) $^ $(FRONT_LIBS) -o $@
test.o: test.c back.h basics.h
	$(FRONT_COMPILE) -c $<

cocoa_main.o: ext/cocoa_main.m ext/../front.h
	$(FRONT_COMPILE) -c $<

front_back.o: front_back.c front.h back.h
basics.o: ext/basics.c ext/../basics.h
	$(BACK_COMPILE) -c $<
