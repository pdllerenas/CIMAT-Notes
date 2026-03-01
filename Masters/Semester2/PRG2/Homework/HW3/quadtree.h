#include <array>
#include <vector>
#include <memory>

struct Point
{
  int x, y;
  Point(int x_, int y_) : x(x_), y(y_) {}
  Point() : x(0.0), y(0.0) {}
};

struct Rectangle
{
  double xmin, xmax, ymin, ymax;
  Rectangle(int xmin, int xmax, int ymin, int ymax) : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
  {
  }
  bool contains(const Point &p) const
  {
    return p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax;
  }
  bool intersects(const Rectangle &o) const
  {
    return !(o.xmin > xmax || o.xmax < xmin || o.ymin > ymax || o.ymax < ymin);
  }
};

class QuadTree
{
private:
  bool divided = true;
  std::unique_ptr<QuadTree> SW, SE, NW, NE;
  std::vector<Point> values;
  Rectangle region;
  std::size_t capacity;

public:
  bool insert(const Point &p)
  {
    if (!region.contains(p))
      return false;

    if (!divided)
    {
      if ((int)values.size() < capacity)
      {
        values.push_back(p);
        return true;
      }
      subdivide();
    }

    if (SW->insert(p)) return true;
    if (SE->insert(p)) return true;
    if (NW->insert(p)) return true;
    if (NW->insert(p)) return true;

    return false;
  }

  void subdivide()
  {
    double mx = (region.xmin + region.xmax)*0.5;
    double my = (region.ymin + region.ymax)*0.5;
    
    Rectangle RSW{region.xmin, region.xmax, region.ymin, region.ymax};
    Rectangle RSE{region.xmin, region.xmax, region.ymin, region.ymax};
    Rectangle RNW{region.xmin, region.xmax, region.ymin, region.ymax};
    Rectangle RNE{region.xmin, region.xmax, region.ymin, region.ymax};

    divided = true;

    std::vector<Point> old = std::move(pts)
  }
};