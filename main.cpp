#include <iostream>

namespace topit
{
  struct p_t
  {
    int x, y;
  };
  struct f_t
  {
    p_t aa, bb;
  };
  size_t rows(f_t fr);
  size_t cols(f_t fr);
  bool operator==(p_t a, p_t b);
  bool operator!=(p_t a, p_t b);
  struct IDraw
  {
    virtual p_t begin() const = 0;
    virtual p_t next(p_t prev) const = 0;
    virtual ~IDraw() = default;
  };
  struct Dot : IDraw
  {
    explicit Dot(p_t dd);

    p_t begin() const override;
    p_t next(p_t prev) const override;
    private:
      p_t d;
  };
  struct VLine: IDraw {
    VLine(p_t top, int length);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    int getLength();
    private:
      int length;
      p_t top;
  };
  struct HLine: IDraw {
    HLine(p_t right, int length);
    p_t begin() const override;
    p_t next(p_t prev) const override;
    private:
      int length;
      p_t right;
  };
  p_t* extend(const p_t* pts, size_t s, p_t fill);
  void extend(p_t**pts, size_t& s, p_t fill);
  void append(const IDraw *sh, p_t **ppts, size_t &s);
  f_t frame(const p_t *pts, size_t s);
  char *canvas(f_t fr, char fill);
  void paint(p_t a, char *cnv, f_t fr, char fill);
  void flush(std::ostream &os, const char *cnv, f_t fr);
}

int main()
{
  using namespace topit;
  IDraw *shp[5] = {};
  p_t *pts = nullptr;
  size_t s = 0;
  int err = 0;
  try
  {
    shp[0] = new Dot({0, 0});
    shp[1] = new Dot({2, 4});
    shp[2] = new Dot({-5, -2});
    shp[3] = new HLine({1, -1}, 4);
    shp[4] = new VLine({4, 2}, 5);
    VLine test = VLine({4, 2}, 5);
    for (size_t i = 0; i < 5; i++)
    {
      append(shp[i], &pts, s);
    }
    f_t fr = frame(pts, s);
    char *cnv = canvas(fr, '.');
    for (size_t i=0; i < s; i++)
    {
      paint(pts[i], cnv, fr, '#');
    }
    flush(std::cout, cnv, fr);
    delete[] cnv;
  }
  catch (...)
  {
    err = 1;
    std::cerr << "Error\n";
  }
  delete shp[0];
  delete shp[1];
  delete shp[2];
  delete shp[3];
  delete shp[4];
  return err;
}

topit::p_t* topit::extend(const p_t* pts, size_t s, p_t fill) {
  p_t* r = new p_t[s+1];
  for (size_t i=0;i<s;i++) {
    r[i] = pts[i];
  }
  r[s] = fill;
  return r;
}
void topit::extend(p_t** pts, size_t& s, p_t fill) {
  p_t* r = extend(*pts, s, fill);
  delete[] *pts;
  ++s;
  *pts=r;
}
void topit::append(const IDraw *sh, p_t **ppts, size_t &s) {
  extend(ppts, s, sh->begin());
  p_t b = sh->begin();
  while (sh->next(b) != sh->begin()) {
    b = sh-> next(b);
    extend(ppts, s, b);
  }
}
topit::f_t topit::frame(const p_t *pts, size_t s)
{
  int minx = pts[0].x;
  int miny = pts[0].y;
  int maxx = minx, maxy = miny;
  for (size_t i = 1; i < s; i++)
  {
    minx = std::min(minx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxx = std::max(maxx, pts[i].x);
    maxy = std::max(maxy, pts[i].y);
  }
  p_t a{minx, miny};
  p_t b{maxx, maxy};
  return {a, b};
}
char *topit::canvas(f_t fr, char fill)
{
  size_t s = rows(fr) * cols(fr);
  char *r = new char[s];
  for (size_t i = 0; i < s; i++)
  {
    r[i] = fill;
  }
  return r;
}
void topit::paint(p_t a, char *cnv, f_t fr, char fill)
{
  size_t dy = fr.bb.y - a.y;
  size_t dx = a.x - fr.aa.x;
  cnv[dy * cols(fr) + dx] = fill;
}
void topit::flush(std::ostream &os, const char *cnv, f_t fr)
{
  for (size_t i = 0; i < rows(fr); i++)
  {
    for (size_t j = 0; j < cols(fr); j++)
    {
      os << cnv[i * cols(fr) + j];
    }
    os << '\n';
  }
}

size_t topit::rows(f_t fr)
{
  return (fr.bb.y - fr.aa.y + 1);
}
size_t topit::cols(f_t fr)
{
  return (fr.bb.x - fr.aa.x + 1);
}

topit::Dot::Dot(p_t dd) :
d(dd)
{}
topit::p_t topit::Dot::begin() const
{
  return d;
}

topit::p_t topit::Dot::next(p_t prev) const
{
  return d;
}

topit::HLine::HLine(p_t right, int length):
  right(right),
  length(length)
 {}
topit::p_t topit::HLine::begin() const {
  return right;
}
topit::p_t topit::HLine::next(p_t b) const {
  if (b.x - 1 > (right.x - length)) {
    return p_t{b.x - 1, right.y};
  }
  return right;
}

topit::VLine::VLine(p_t top, int length):
  top(top), length(length)
  {}
topit::p_t topit::VLine::begin() const{
  return top;
}
topit::p_t topit::VLine::next(p_t b) const {
  if (b.y-1 > (top.y-length)) {
    return p_t{top.x, b.y-1};
  }
  return top;
}
int topit::VLine::getLength() {
  return length;
}
bool topit::operator==(p_t a, p_t b)
{
  return a.x == b.x && a.y == b.y;
}

bool topit::operator!=(p_t a, p_t b)
{
  return !(a == b);
}
