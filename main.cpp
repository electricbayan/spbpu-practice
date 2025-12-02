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
  bool operator==(p_t a, p_t b);
  bool operator!=(p_t a, p_t b);
  struct IDraw {
    virtual p_t next() const=0;
    virtual p_t next(p_t prev) const=0;
    virtual ~IDraw() = default;
  };
  struct Dot: IDraw {
    explicit Dot(p_t dd);
    p_t d;

    p_t next() const override;
    p_t next(p_t prev) const override;

  };
}

int main()
{
  using namespace topit;
  IDraw* shp[3] = {};
  int err=0;
  try{
    shp[0] = new Dot({0, 0});
  } catch (...) {
    err=1;
  }
  delete[] shp[0];
  return err;
}

topit::Dot::Dot(p_t dd):
  d(dd)
{}
topit::p_t topit::Dot::next() const {
  return d;
}

topit::p_t topit::Dot::next(p_t prev) const {
  return d;
}
bool topit::operator==(p_t a, p_t b)
{
  return a.x == b.x && a.y == b.y;
}

bool topit::operator!=(p_t a, p_t b) {
  return !(a==b);
}
