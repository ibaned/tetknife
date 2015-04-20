#include "draw.h"
#include "basics.h"
#include "charbits.h"
#include "comm.h"

static double const line_offset = 4.0;
static double const tri_offset = 8.0;

void drawing_init(drawing* d, unsigned w, unsigned h)
{
  unsigned i;
  image_init(&d->im, w, h);
  d->z = my_malloc(h * sizeof(double*));
  d->z[0] = my_malloc(w * h * sizeof(double));
  for (i = 1; i < h; ++i)
    d->z[i] = d->z[i - 1] + w;
}

void drawing_dtor(drawing* d)
{
  image_dtor(&d->im);
  my_free(d->z[0]);
  my_free(d->z);
}

void drawing_clear(drawing* d, color c)
{
  unsigned i;
  unsigned j;
  image_clear(&d->im, c);
  for (i = 0; i < d->im.h; ++i)
    for (j = 0; j < d->im.w; ++j)
      d->z[i][j] = my_dbl_max;
}

static pix point_pix(point p)
{
  pix px;
  px.x = (int)p.x;
  px.y = (int)p.y;
  return px;
}

static void draw_pix(drawing* dr, pix p, double z, color c)
{
  double* zp = &dr->z[p.y][p.x];
  if (z <= *zp) {
    *zp = z;
    dr->im.p[p.y][p.x] = c;
  }
}

static pix pix_new(int x, int y)
{
  pix p;
  p.x = x;
  p.y = y;
  return p;
}

static pix pix_min(pix a, pix b)
{
  return pix_new(MIN(a.x, b.x), MIN(a.y, b.y));
}

static pix pix_max(pix a, pix b)
{
  return pix_new(MAX(a.x, b.x), MAX(a.y, b.y));
}

static int pix_geq(pix a, pix b)
{
  return a.x >= b.x && a.y >= b.y;
}

static pix pix_sub(pix a, pix b)
{
  return pix_new(a.x - b.x, a.y - b.y);
}

static pix pix_add(pix a, pix b)
{
  return pix_new(a.x + b.x, a.y + b.y);
}

static pix pix_perp(pix p)
{
  return pix_new(-p.y, p.x);
}

static int pix_dot(pix a, pix b)
{
  return a.x * b.x + a.y * b.y;
}

typedef struct {
  pix ul;
  pix lr;
} pixbox;

static pixbox pixbox_new(pix ul, pix lr)
{
  pixbox pb;
  pb.ul = ul;
  pb.lr = lr;
  return pb;
}

static pixbox image_pixbox(image* im)
{
  return pixbox_new(pix_new(0, 0), pix_new((int)im->w - 1, (int)im->h - 1));
}

static pixbox pixbox_intersect(pixbox a, pixbox b)
{
  return pixbox_new(pix_max(a.ul, b.ul), pix_min(a.lr, b.lr));
}

static int pixbox_empty(pixbox b)
{
  return !pix_geq(b.lr, b.ul);
}

typedef struct {
  pix a;
  pix b;
} pixline;

static pixline line_pixline(line l)
{
  pixline pl;
  pl.a = point_pix(l.a);
  pl.b = point_pix(l.b);
  return pl;
}

static pixbox pixline_pixbox(pixline l)
{
  return pixbox_new(pix_min(l.a, l.b),
                    pix_max(l.a, l.b));
}

typedef struct {
  pix v;
  pix n;
  pix o;
  int lsq;
} linefrm;

static linefrm pixline_frame(pixline l)
{
  linefrm f;
  f.v = pix_sub(l.b, l.a);
  f.n = pix_perp(f.v);
  f.o.x = pix_dot(f.v, l.a);
  f.o.y = pix_dot(f.n, l.a);
  f.lsq = pix_dot(f.v, f.v);
  return f;
}

static pix linefrm_uneval(linefrm f, pix p)
{
  return pix_sub(pix_new(pix_dot(p, f.v), pix_dot(p, f.n)), f.o);
}

typedef struct {
  pix a;
  pix b;
  pix c;
} pixtri;

static pixtri triangle_pixtri(triangle t)
{
  pixtri pt;
  pt.a = point_pix(t.a);
  pt.b = point_pix(t.b);
  pt.c = point_pix(t.c);
  return pt;
}

static pixbox pixtri_pixbox(pixtri t)
{
  pixbox bb;
  bb.ul = pix_min(pix_min(t.a, t.b), t.c);
  bb.lr = pix_max(pix_max(t.a, t.b), t.c);
  return bb;
}

typedef struct {
  int x;
  int y;
  int z;
} vox;

static vox vox_sub(vox a, vox b)
{
  vox c;
  c.x = a.x - b.x;
  c.y = a.y - b.y;
  c.z = a.z - b.z;
  return c;
}

typedef struct {
  pix na;
  pix nb;
  pix nc;
  vox o;
  int a2;
} trifrm;

static int pixtri_area2(pixtri t)
{
  return pix_dot(pix_perp(pix_sub(t.b, t.a)), pix_sub(t.c, t.a));
}

static trifrm pixtri_frame(pixtri t)
{
  trifrm f;
  f.na = pix_perp(pix_sub(t.c, t.b));
  f.nb = pix_perp(pix_sub(t.a, t.c));
  f.nc = pix_perp(pix_sub(t.b, t.a));
  f.o.x = pix_dot(f.na, t.b);
  f.o.y = pix_dot(f.nb, t.c);
  f.o.z = pix_dot(f.nc, t.a);
  f.a2 = pixtri_area2(t);
  return f;
}

static vox trifrm_uneval(trifrm f, pix p)
{
  vox v;
  v.x = pix_dot(f.na, p);
  v.y = pix_dot(f.nb, p);
  v.z = pix_dot(f.nc, p);
  return vox_sub(v, f.o);
}

static void draw_point2(drawing* d, point p, color c, int thick)
{
  pixbox bb;
  pix q;
  bb.ul = point_pix(p);
  bb.lr = pix_add(bb.ul, pix_new(thick - 1, thick - 1));
  bb = pixbox_intersect(bb, image_pixbox(&d->im));
  for (q.y = bb.ul.y; q.y <= bb.lr.y; ++q.y)
    for (q.x = bb.ul.x; q.x <= bb.lr.x; ++q.x)
      draw_pix(d, q, p.z, c);
}

void draw_point(drawing* d, point p, color c)
{
  draw_point2(d, p, c, 4);
}

void draw_line(drawing* d, line l, color c)
{
  pixline pl;
  pixbox bb;
  linefrm f;
  pix q;
  pix e;
  int tol;
  double z;
  pl = line_pixline(l);
  bb = pixline_pixbox(pl);
  bb = pixbox_intersect(bb, image_pixbox(&d->im));
  if (pixbox_empty(bb))
    return;
  f = pixline_frame(pl);
  if (!f.lsq) {
    draw_point2(d, l.a, c, 1);
    draw_point2(d, l.b, c, 1);
    return;
  }
  tol = MAX(ABS(f.n.x), ABS(f.n.y)) / 2;
  for (q.y = bb.ul.y; q.y <= bb.lr.y; ++q.y)
    for (q.x = bb.ul.x; q.x <= bb.lr.x; ++q.x) {
      e = linefrm_uneval(f, q);
      if (ABS(e.y) <= tol) {
        z = ((f.lsq - e.x) * l.a.z + e.x * l.b.z) / f.lsq;
        z += line_offset;
        draw_pix(d, q, z, c);
      }
    }
}

static void flip(triangle* t)
{
  point tmp;
  tmp = t->a;
  t->a = t->b;
  t->b = tmp;
}

void draw_triangle(drawing* d, triangle t, color c)
{
  pixtri pt;
  pixbox bb;
  trifrm f;
  pix q;
  vox e;
  double z;
  pt = triangle_pixtri(t);
  bb = pixtri_pixbox(pt);
  bb = pixbox_intersect(bb, image_pixbox(&d->im));
  if (pixbox_empty(bb))
    return;
  if (pixtri_area2(pt) < 0) {
    flip(&t);
    pt = triangle_pixtri(t);
  }
  f = pixtri_frame(pt);
  if (!f.a2) {
    draw_line(d, line_new(t.a, t.b), c);
    draw_line(d, line_new(t.b, t.c), c);
    draw_line(d, line_new(t.c, t.a), c);
    return;
  }
  q = bb.ul;
  for (q.y = bb.ul.y; q.y <= bb.lr.y; ++q.y)
    for (q.x = bb.ul.x; q.x <= bb.lr.x; ++q.x) {
      e = trifrm_uneval(f, q);
      if (e.x >= 0 && e.y >= 0 && e.z >= 0) {
        z = (e.x * t.a.z + e.y * t.b.z + e.z * t.c.z) / f.a2;
        z += tri_offset;
        draw_pix(d, q, z, c);
      }
    }
}

static void draw_char(drawing* dr, pix p, char ch, color c)
{
  unsigned char const* bits;
  int i,j;
  if (((unsigned)p.x + CHARBITS_WIDTH >= dr->im.w) ||
      ((unsigned)p.y + CHARBITS_HEIGHT >= dr->im.h))
    return;
  bits = charbits(ch);
  for (i = 0; i < CHARBITS_HEIGHT; ++i)
  for (j = 0; j < CHARBITS_WIDTH; ++j)
    if (bits[i] & (1<<j))
      draw_pix(dr, pix_new(p.x + j, p.y + i), -my_dbl_max, c);
}

void draw_text(drawing* dr, const char* s, point where, color c)
{
  pix p;
  int ix;
  p = point_pix(where);
  if (p.x <= 0 || p.y <= 0)
    return;
  ix = p.x;
  for (; *s; ++s) {
    if (*s == '\n') {
      p.y += CHARBITS_HEIGHT;
      p.x = ix;
    } else {
      draw_char(dr, p, *s, c);
      p.x += CHARBITS_WIDTH;
    }
  }
}

static unsigned global_npixels;

static void drawing_reduce_op(void* inout, void const* in)
{
  unsigned sizez;
  color* ioc;
  color const* ic;
  double* ioz;
  double const* iz;
  unsigned i;
  sizez = global_npixels * sizeof(double);
  ioz = inout;
  iz = in;
  ioc = (color*)(((char*)inout) + sizez);
  ic = (color*)(((char*)in) + sizez);
  for (i = 0; i < global_npixels; ++i)
    if (iz[i] <= ioz[i])
      ioc[i] = ic[i];
}

void drawing_reduce(drawing* d)
{
  char* data;
  unsigned size;
  unsigned sizez;
  global_npixels = d->im.w * d->im.h;
  /* we put the Z buffer in first because doubles
     require 8-byte alignment, which they wouldn't
     necessarily get if packed after the end
     of the colors */
  sizez = global_npixels * sizeof(double);
  size = sizez + global_npixels * sizeof(color);
  data = my_malloc(size);
  my_memcpy(data, d->z[0], sizez);
  my_memcpy(data + sizez, d->im.p[0], size - sizez);
  mpi_reduce(comm_mpi(), data, size, drawing_reduce_op);
  my_free(data);
}
