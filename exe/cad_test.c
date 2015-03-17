#include "../cad.h"
#include "../cad_gen.h"
#include "../cad_geom.h"
#include "../basics.h"

static void print_ent(cad* c, gent e)
{
  gbnd b;
  guse u;
  debug("(%s, %d) {%s}\n", gent_name[e.t], e.i, geom_name[geom_typeof(c, e)]);
  for (b = gbnd_of_f(c, e); gbnd_ok(b); b = gbnd_of_n(c, b)) {
    debug(" (%s, %d)\n ", guse_name[b.t], b.i);
    for (u = guse_by_f(c, b); guse_ok(u); u = guse_by_n(c, u)) {
      e = guse_of(c, u);
      debug(" (%s, %d)", gent_name[e.t], e.i);
    }
    debug("\n");
  }
}

static void print_model(cad* c)
{
  gent_type t;
  gent e;
  for (t = 0; t < CAD_ENT_TYPES; ++t) {
    debug("%d %s's\n", gent_count(c, t), gent_name[t]);
    for (e = gent_f(c, t); gent_ok(e); e = gent_n(c, e))
      print_ent(c, e);
  }
}

int main()
{
  point const pts[4] = {
    {0,0,0},
    {1,0,0},
    {1,1,0},
    {0,1,0}
  };
  cad* c;
  c = cad_new();
  geom_new(c);
  cad_gen_polygon(c, 4, pts);
  print_model(c);
  cad_free(c);
  return 0;
}
