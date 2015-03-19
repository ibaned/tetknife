CONFIG ?= default

include $(CONFIG).mk

BACK_OBJS = \
mesh_mod.o \
mesh_geom.o \
mesh_adj.o \
mesh_bbox.o \
classif.o \
cad_gen.o \
cad_geom.o \
cad.o \
list.o \
flag.o \
field.o \
mesh.o \
flex.o \
stack.o \
param.o \
simplex.o \
space.o \
basics.o

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
cad_test: cad_test.o $(BACK_OBJS)

test.o: exe/test.c back.h basics.h
	$(FRONT_COMPILE) -c $<
cad_test.o: exe/cad_test.c exe/../cad.h
	$(BACK_COMPILE) -c $<

cocoa_main.o: ext/cocoa_main.m front.h
	$(FRONT_COMPILE) -c $<

basics.o: ext/basics.c basics.h
	$(BACK_COMPILE) -c $<

cad.o: cad.c cad.h space.h flex.h stack.h basics.h list.h flag.h \
  cad_geom.h param.h
cad_gen.o: cad_gen.c cad_gen.h cad.h space.h cad_geom.h param.h
cad_geom.o: cad_geom.c cad_geom.h cad.h space.h param.h flex.h stack.h \
  basics.h simplex.h
classif.o: classif.c classif.h mesh.h simplex.h space.h cad.h list.h \
  field.h basics.h cad_geom.h param.h
field.o: field.c field.h mesh.h simplex.h space.h basics.h
flag.o: flag.c flag.h cad.h space.h basics.h
flex.o: flex.c flex.h stack.h basics.h
front_back.o: front_back.c front.h back.h
list.o: list.c list.h basics.h
mesh.o: mesh.c mesh.h simplex.h space.h flex.h stack.h basics.h list.h \
  field.h classif.h cad.h
mesh_adj.o: mesh_adj.c mesh_adj.h mesh.h simplex.h space.h stack.h \
  basics.h
mesh_bbox.o: mesh_bbox.c mesh_bbox.h mesh.h simplex.h space.h cad.h \
  classif.h
mesh_mod.o: mesh_mod.c mesh_mod.h mesh.h simplex.h space.h mesh_adj.h \
  stack.h basics.h classif.h cad.h
param.o: param.c param.h space.h basics.h
simplex.o: simplex.c simplex.h space.h
space.o: space.c space.h basics.h
stack.o: stack.c stack.h basics.h
