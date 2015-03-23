CONFIG ?= default

include $(CONFIG).mk

ifeq "$(USE_MPI)" "yes"
MPI_OBJ = my_mpi.o
else
MPI_OBJ = fake_mpi.o
endif

BACK_OBJS = \
rib.o \
view_mesh.o \
view.o \
draw.o \
charbits.o \
image.o \
mesh_adapt.o \
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
ibarrier.o \
$(MPI_OBJ) \
basics.o

ifeq "$(GUI)" "cocoa"
GUI_OBJ += main_cocoa.o
GUI_LIBS = -framework Cocoa
else ifeq "$(GUI)" "gtk"
GTK_FLAGS = $(shell pkg-config --cflags gtk+-2.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-2.0)
GUI_OBJ += main_gtk.o
GUI_LIBS = $(GTK_LIBS)
else ifeq "$(GUI)" "w32"
GUI_OBJ += main_w32.o
GUI_LIBS = -lGdi32
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
myperf: myperf.o $(BACK_OBJS)
cad_test: cad_test.o $(BACK_OBJS)
quality: quality.o $(BACK_OBJS)
cubic: cubic.o $(BACK_OBJS)
rib_test: rib_test.o $(BACK_OBJS)
libxmesh.a: $(BACK_OBJS)
	ar cru $@ $^

test.o: exe/test.c exe/../back.h exe/../view_mesh.h exe/../view.h \
  exe/../simplex.h exe/../space.h exe/../image.h exe/../mesh.h \
  exe/../mesh_bbox.h exe/../cad.h exe/../mesh_adapt.h exe/../flag.h \
  exe/../mesh_geom.h exe/../mesh_adj.h exe/../stack.h exe/../basics.h
	$(FRONT_COMPILE) -c $<
myperf.o: exe/myperf.c exe/../view_mesh.h exe/../view.h exe/../simplex.h \
  exe/../space.h exe/../image.h exe/../mesh.h exe/../mesh_bbox.h \
  exe/../cad.h exe/../mesh_adapt.h exe/../flag.h
	$(FRONT_COMPILE) -c $<
cad_test.o: exe/cad_test.c exe/../cad.h
	$(BACK_COMPILE) -c $<
quality.o: exe/quality.c exe/../simplex.h exe/../space.h exe/../basics.h \
  exe/../mesh_geom.h exe/../mesh.h
	$(BACK_COMPILE) -c $<
cubic.o: exe/cubic.c exe/../basics.h
	$(BACK_COMPILE) -c $<
rib_test.o: exe/rib_test.c exe/../rib.h exe/../space.h exe/../basics.h
	$(BACK_COMPILE) -c $<

main_cocoa.o: ext/main_cocoa.m front.h
	$(FRONT_COMPILE) -c $<
main_gtk.o: ext/main_gtk.c ext/../front.h
	$(FRONT_COMPILE) $(GTK_FLAGS) -c $<
main_w32.o: ext/main_w32.c ext/../front.h
	$(FRONT_COMPILE) -c $<
basics.o: ext/basics.c basics.h
	$(BACK_COMPILE) -c $<
my_mpi.o: ext/my_mpi.c ext/../basics.h ext/../my_mpi.h
	$(BACK_COMPILE) -c $<

cad.o: cad.c cad.h space.h flex.h stack.h basics.h list.h flag.h mesh.h \
  simplex.h cad_geom.h param.h
cad_gen.o: cad_gen.c cad_gen.h cad.h space.h cad_geom.h param.h
cad_geom.o: cad_geom.c cad_geom.h cad.h space.h param.h flex.h stack.h \
  basics.h simplex.h
charbits.o: charbits.c charbits.h
classif.o: classif.c classif.h mesh.h simplex.h space.h cad.h list.h \
  field.h basics.h cad_geom.h param.h mesh_adj.h stack.h
draw.o: draw.c draw.h image.h simplex.h space.h basics.h charbits.h
fake_mpi.o: fake_mpi.c my_mpi.h basics.h
field.o: field.c field.h mesh.h simplex.h space.h basics.h
flag.o: flag.c flag.h cad.h space.h mesh.h simplex.h basics.h
flex.o: flex.c flex.h stack.h basics.h
front_back.o: front_back.c front.h back.h
ibarrier.o: ibarrier.c ibarrier.h my_mpi.h
image.o: image.c image.h basics.h
list.o: list.c list.h basics.h
mesh.o: mesh.c mesh.h simplex.h space.h flex.h stack.h basics.h list.h \
  field.h classif.h cad.h flag.h
mesh_adapt.o: mesh_adapt.c mesh_adapt.h mesh.h simplex.h space.h flag.h \
  cad.h mesh_adj.h stack.h basics.h mesh_mod.h
mesh_adj.o: mesh_adj.c mesh_adj.h mesh.h simplex.h space.h stack.h \
  basics.h
mesh_bbox.o: mesh_bbox.c mesh_bbox.h mesh.h simplex.h space.h cad.h \
  classif.h
mesh_geom.o: mesh_geom.c mesh_geom.h mesh.h simplex.h space.h mesh_adj.h \
  stack.h basics.h field.h
mesh_mod.o: mesh_mod.c mesh_mod.h mesh.h simplex.h space.h mesh_adj.h \
  stack.h basics.h mesh_geom.h classif.h cad.h
param.o: param.c param.h space.h basics.h
rib.o: rib.c rib.h space.h basics.h
simplex.o: simplex.c simplex.h space.h
space.o: space.c space.h basics.h
stack.o: stack.c stack.h basics.h
view.o: view.c view.h simplex.h space.h image.h draw.h basics.h
view_mesh.o: view_mesh.c view_mesh.h view.h simplex.h space.h image.h \
  mesh.h mesh_geom.h mesh_adj.h stack.h basics.h
