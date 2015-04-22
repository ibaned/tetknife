CONFIG ?= config

include $(CONFIG).mk

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
migrate.o \
remotes.o \
label.o \
flag.o \
field.o \
mesh.o \
flex.o \
stack.o \
param.o \
luby.o \
mersenne_twister.o \
simplex.o \
space.o \
comm.o \
ibarrier.o \
my_endian.o \
basics.o

ifeq "$(USE_MPI)" "yes"
BACK_OBJS += my_mpi.o
else ifeq "$(USE_MPI)" "no"
BACK_OBJS += serial_mpi.o
else
$(error USE_MPI must be either "yes" or "no")
endif

ifeq "$(GUI)" "cocoa"
GUI_OBJ = main_cocoa.o
GUI_LIBS = -framework Cocoa
else ifeq "$(GUI)" "gtk"
GTK_FLAGS = $(shell pkg-config --cflags gtk+-2.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-2.0)
GUI_OBJ = main_gtk.o
GUI_LIBS = $(GTK_LIBS)
else ifeq "$(GUI)" "w32"
GUI_OBJ = main_w32.o
GUI_LIBS = -lGdi32
else ifeq "$(GUI)" "none"
else
$(error GUI must be "cocoa", "gtk", "w32", or "none")
endif

ifeq "$(BACK)" "direct"
GUI_BACK_OBJS = $(GUI_OBJ) front_back.o $(BACK_OBJS)
GUI_BACK_LIBS = $(GUI_LIBS) $(LDLIBS)
FRONT_CC = $(CC)
FRONT_LDFLAGS = $(LDFLAGS)
FRONT_CFLAGS = $(CFLAGS)
  ifneq "$(SOCKET)" "none"
  $(error SOCKET must be "none" when BACK is "direct")
  endif
else ifeq "$(BACK)" "socket"
  ifeq "$(SOCKET)" "posix"
    SERVER_OBJ = server_posix.o
    CLIENT_OBJ = client_posix.o
    RELAY_OBJ = relay_posix.o
  else ifeq "$(SOCKET)" "w32"
    SERVER_OBJ = server_w32.o
    CLIENT_OBJ = client_w32.o
  else
  $(error SOCKET must be either "posix" or "w32")
  endif
GUI_BACK_OBJS = \
$(CLIENT_OBJ) \
main_socket_back.o \
$(BACK_OBJS)
GUI_BACK_LIBS = $(LDLIBS)
FRONT_CC ?= $(CC)
FRONT_OBJS = \
$(GUI_OBJ) \
socket_front.o \
front_image.o \
$(SERVER_OBJ) \
front_endian.o \
front_basics.o
FRONT_LIBS ?= $(LDLIBS)
FRONT_LIBS += $(GUI_LIBS)
FRONT_LDFLAGS ?= $(LDFLAGS)
FRONT_CFLAGS ?= $(CFLAGS)
MID_CC ?= $(CC)
MID_OBJS = \
$(RELAY_OBJ) \
mid_basics.o
MID_CFLAGS ?= $(CFLAGS)
else
$(error BACK must be either "direct" or "socket")
endif

FRONT_LINK = $(FRONT_CC) $(FRONT_LDFLAGS)
FRONT_COMPILE = $(FRONT_CC) $(FRONT_CFLAGS)
BACK_LINK = $(CC) $(LDFLAGS)
BACK_COMPILE = $(CC) $(CFLAGS)
MID_LINK = $(MID_CC) $(MID_LDFLAGS)
MID_COMPILE = $(MID_CC) $(MID_CFLAGS)

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

viewer: $(FRONT_OBJS)
	$(FRONT_LINK) $^ $(FRONT_LIBS) -o $@
relay: $(MID_OBJS)
	$(MID_LINK) $^ $(MID_LIBS) -o $@
refine_test: refine_test.o $(GUI_BACK_OBJS)
	$(BACK_LINK) $^ $(GUI_BACK_LIBS) -o $@
full_test: full_test.o $(GUI_BACK_OBJS)
	$(BACK_LINK) $^ $(GUI_BACK_LIBS) -o $@
reduce_test: reduce_test.o $(GUI_BACK_OBJS)
	$(BACK_LINK) $^ $(GUI_BACK_LIBS) -o $@
myperf: myperf.o $(BACK_OBJS)
cad_test: cad_test.o $(BACK_OBJS)
quality: quality.o $(BACK_OBJS)
cubic: cubic.o $(BACK_OBJS)
rib_test: rib_test.o $(BACK_OBJS)
commtest: commtest.o $(BACK_OBJS)
migrtest: migrtest.o $(BACK_OBJS)
mersenne_test: mersenne_test.o $(BACK_OBJS)
luby_test: luby_test.o $(BACK_OBJS)

full_test.o: exe/full_test.c back.h view_mesh.h \
  view.h simplex.h space.h image.h \
  mesh.h mesh_bbox.h cad.h mesh_adapt.h \
  flag.h mesh_geom.h mesh_adj.h stack.h \
  basics.h
	$(BACK_COMPILE) -c $<
refine_test.o: exe/refine_test.c back.h view_mesh.h view.h \
  simplex.h space.h image.h mesh.h \
  mesh_bbox.h cad.h mesh_adapt.h flag.h \
  mesh_geom.h mesh_adj.h stack.h basics.h
	$(BACK_COMPILE) -c $<
reduce_test.o: exe/reduce_test.c back.h draw.h \
  image.h simplex.h space.h comm.h \
  my_mpi.h luby.h mesh.h
	$(BACK_COMPILE) -c $<
myperf.o: exe/myperf.c view_mesh.h view.h simplex.h \
  space.h image.h mesh.h mesh_bbox.h \
  cad.h mesh_adapt.h flag.h
	$(BACK_COMPILE) -c $<
cad_test.o: exe/cad_test.c cad.h
	$(BACK_COMPILE) -c $<
quality.o: exe/quality.c simplex.h space.h basics.h \
  mesh_geom.h mesh.h
	$(BACK_COMPILE) -c $<
cubic.o: exe/cubic.c basics.h
	$(BACK_COMPILE) -c $<
rib_test.o: exe/rib_test.c rib.h space.h basics.h
	$(BACK_COMPILE) -c $<
commtest.o: exe/commtest.c comm.h my_mpi.h basics.h
	$(BACK_COMPILE) -c $<
migrtest.o: exe/migrtest.c migrate.h mesh.h \
  simplex.h space.h label.h mesh_bbox.h \
  cad.h comm.h my_mpi.h
	$(BACK_COMPILE) -c $<
mersenne_test.o: exe/mersenne_test.c mersenne_twister.h basics.h
	$(BACK_COMPILE) -c $<
luby_test.o: exe/luby_test.c luby.h
	$(BACK_COMPILE) -c $<

main_cocoa.o: ext/main_cocoa.m front.h
	$(FRONT_COMPILE) -c $<
main_gtk.o: ext/main_gtk.c front.h
	$(FRONT_COMPILE) $(GTK_FLAGS) -c $<
main_w32.o: ext/main_w32.c front.h
	$(FRONT_COMPILE) -c $<
server_posix.o: ext/server_posix.c server.h basics.h ext/rw_posix.h
	$(FRONT_COMPILE) -c $<
client_posix.o: ext/client_posix.c client.h basics.h ext/rw_posix.h
	$(BACK_COMPILE) -c $<
relay_posix.o: ext/relay_posix.c basics.h ext/client_posix.h \
  ext/rw_posix.h ext/server_posix.h
	$(MID_COMPILE) -c $<
server_w32.o: ext/server_w32.c server.h basics.h ext/socket_w32.h
	$(FRONT_COMPILE) -c $<
client_w32.o: ext/client_w32.c client.h basics.h ext/socket_w32.h
	$(BACK_COMPILE) -c $<
basics.o: ext/basics.c basics.h
	$(BACK_COMPILE) -c $<
front_basics.o: ext/basics.c basics.h
	$(FRONT_COMPILE) -c $< -o $@
mid_basics.o: ext/basics.c basics.h
	$(MID_COMPILE) -c $< -o $@
front_endian.o: my_endian.c my_endian.h
	$(FRONT_COMPILE) -c $< -o $@
front_image.o: image.c image.h basics.h
	$(FRONT_COMPILE) -c $< -o $@
socket_front.o: socket_front.c front.h server.h image.h my_endian.h \
  basics.h socket_codes.h
	$(FRONT_COMPILE) -c $< -o $@
my_mpi.o: ext/my_mpi.c basics.h my_mpi.h
	$(BACK_COMPILE) -c $<

cad.o: cad.c cad.h space.h flex.h stack.h basics.h list.h flag.h mesh.h \
  simplex.h cad_geom.h param.h
cad_gen.o: cad_gen.c cad_gen.h cad.h space.h cad_geom.h param.h
cad_geom.o: cad_geom.c cad_geom.h cad.h space.h param.h flex.h stack.h \
  basics.h simplex.h
charbits.o: charbits.c charbits.h
classif.o: classif.c classif.h mesh.h simplex.h space.h cad.h list.h \
  field.h basics.h cad_geom.h param.h mesh_adj.h stack.h
comm.o: comm.c comm.h my_mpi.h ibarrier.h basics.h stack.h
draw.o: draw.c draw.h image.h simplex.h space.h basics.h charbits.h
field.o: field.c field.h mesh.h simplex.h space.h basics.h
flag.o: flag.c flag.h cad.h space.h mesh.h simplex.h basics.h
flex.o: flex.c flex.h stack.h basics.h
front_back.o: front_back.c front.h back.h
ibarrier.o: ibarrier.c ibarrier.h my_mpi.h
image.o: image.c image.h basics.h
label.o: label.c label.h mesh.h simplex.h space.h basics.h
list.o: list.c list.h basics.h
main_socket_back.o: main_socket_back.c back.h client.h my_endian.h \
  basics.h socket_codes.h
mersenne_twister.o: mersenne_twister.c mersenne_twister.h basics.h
mesh.o: mesh.c mesh.h simplex.h space.h flex.h stack.h basics.h list.h \
  field.h classif.h cad.h flag.h remotes.h
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
migrate.o: migrate.c migrate.h mesh.h simplex.h space.h label.h comm.h \
  my_mpi.h remotes.h mesh_adj.h stack.h basics.h
luby.o: luby.c luby.h comm.h my_mpi.h mersenne_twister.h \
  basics.h
my_endian.o: my_endian.c my_endian.h
param.o: param.c param.h space.h basics.h
remotes.o: remotes.c remotes.h mesh.h simplex.h space.h flex.h stack.h \
  basics.h comm.h my_mpi.h
rib.o: rib.c rib.h space.h remotes.h mesh.h simplex.h basics.h comm.h \
  my_mpi.h migrate.h label.h mesh_geom.h mesh_adj.h stack.h
serial_mpi.o: serial_mpi.c my_mpi.h basics.h
simplex.o: simplex.c simplex.h space.h
space.o: space.c space.h basics.h
stack.o: stack.c stack.h basics.h
view.o: view.c view.h simplex.h space.h image.h draw.h basics.h
view_mesh.o: view_mesh.c view_mesh.h view.h simplex.h space.h image.h \
  mesh.h mesh_geom.h mesh_adj.h stack.h basics.h
