#ifndef CAD_GEN_H
#define CAD_GEN_H

#include "cad.h"

gent cad_gen_point(cad* c, point p);
gent cad_gen_edge(cad* c, gent v0, gent v1);
gbnd cad_gen_loop(cad* c, unsigned npts, point const pts[]);
gbnd cad_gen_line_loop(cad* c, unsigned npts, point const pts[]);
gbnd cad_gen_circle(cad* c, point o, point n, point x);
gent cad_gen_plane(cad* c, gbnd b);
gent cad_gen_polygon(cad* c, unsigned npts, point const pts[]);
gent cad_gen_disk(cad* c, point o, point n, point x);
void cad_extrude_face(cad* c, gent f, point n);

#endif
