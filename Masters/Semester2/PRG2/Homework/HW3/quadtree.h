#include <array>
#include <vector>
#include <memory>
#include <iostream>

constexpr std::size_t CAPACITY = 1;

struct Point
{
  double x, y;
  Point(double x_, double y_) : x(x_), y(y_) {}
  Point() : x(0.0), y(0.0) {}
};

struct Rectangle
{
  double xmin, xmax, ymin, ymax;
  Rectangle(double xmin, double xmax, double ymin, double ymax) : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax)
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
  bool divided_ = false;
  std::unique_ptr<QuadTree> SW_, SE_, NW_, NE_;
  std::vector<Point> values_;
  Rectangle region_;
  std::size_t capacity_;
  std::size_t depth_;

  void subdivide()
  {
    double mx = (region_.xmin + region_.xmax) * 0.5;
    double my = (region_.ymin + region_.ymax) * 0.5;

    SW_ = std::make_unique<QuadTree>(Rectangle{region_.xmin, mx, region_.ymin, my}, depth_ + 1, capacity_);
    SE_ = std::make_unique<QuadTree>(Rectangle{mx, region_.xmax, region_.ymin, my}, depth_ + 1, capacity_);
    NW_ = std::make_unique<QuadTree>(Rectangle{region_.xmin, mx, my, region_.ymax}, depth_ + 1, capacity_);
    NE_ = std::make_unique<QuadTree>(Rectangle{mx, region_.xmax, my, region_.ymax}, depth_ + 1, capacity_);

    divided_ = true;

    for (const auto &p : values_)
    {
      SW_->insert(p) || SE_->insert(p) || NW_->insert(p) || NE_->insert(p);
    }
    values_.clear();
  }

public:
  QuadTree(Rectangle region, std::size_t depth) : region_(region), capacity_(CAPACITY), values_(), depth_(depth) {}
  QuadTree(Rectangle region, std::size_t depth, std::size_t capacity) : region_(region), capacity_(capacity), values_(), depth_(depth) {}
  QuadTree(Rectangle region, std::size_t depth, std::size_t capacity, std::vector<Point> points) : region_(region), capacity_(capacity), values_(), depth_(depth) {}

  /*
    counts the number of subdivisons by recurively checking the divided_ property. This is O(n)
  */
  std::size_t count_subdivisions() const
  {
    std::size_t count = divided_ ? 1 : 0;
    if (divided_)
    {
      count += SW_->count_subdivisions();
      count += SE_->count_subdivisions();
      count += NW_->count_subdivisions();
      count += NE_->count_subdivisions();
    }
    return count;
  }

  std::size_t get_max_depth() const
  {
    if (!divided_)
    {
      return depth_;
    }

    std::size_t max_depth = depth_;
    max_depth = std::max(max_depth, SW_->get_max_depth());
    max_depth = std::max(max_depth, SE_->get_max_depth());
    max_depth = std::max(max_depth, NW_->get_max_depth());
    max_depth = std::max(max_depth, NE_->get_max_depth());

    return max_depth;
  }

  /*
  Inserts into the tree in O(log n)
  */
  bool insert(const Point &p)
  {
    if (!region_.contains(p))
      return false;

    if (!divided_)
    {
      if (values_.size() < capacity_)
      {
        values_.push_back(p);
        return true;
      }
      subdivide();
    }

    return SW_->insert(p) || SE_->insert(p) || NW_->insert(p) || NE_->insert(p);
  }

  void print()
  {
    std::cout << "Depth: " << depth_ << std::endl;
    std::cout << "[" << region_.xmin << ',' << region_.xmax << ',' << region_.ymin << ',' << region_.ymax << "]" << std::endl;
    for (const Point &p : values_)
    {
      std::cout << "(" << p.x << "," << p.y << ")" << std::endl;
    }
    if (!values_.size())
      std::cout << "No values" << std::endl;
    if (SW_)
      SW_->print();
    if (SE_)
      SE_->print();
    if (NW_)
      NW_->print();
    if (NE_)
      NE_->print();
  }
};
