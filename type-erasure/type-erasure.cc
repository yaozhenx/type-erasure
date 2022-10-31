// Type Erasure Sample Code. Single File Version.
//
// Based on "Breaking Dependencies: Type Erasure - A Design Analysis"
// by Klaus Iglberger, CppCon 2021.
//
// References:
// - https://meetingcpp.com/mcpp/slides/2021/Type%20Erasure%20-%20A%20Design%20Analysis9268.pdf
// - https://www.youtube.com/watch?v=4eeESJQk-mw

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// CAUTION: The following forward declarations prevent the compiler from
// complaining about missing global functions serialize() and draw() when
// seeing the using declarations in ShapeModel::serialize() and
// ShapeModel::draw(), as if the compiler did not see the `friend` definitions
// within `class Shape`.
class Shape;
void serialize(const Shape& shape);
void draw(const Shape& shape);

// Deleting the following 2 function templates to avoid runaway recursion in
// case a concrete Shape such as `Cicle` does not define a
// `serialize(const Circle&)` or `draw(const Circle&)` function.
template <typename T>
void serialize(const T&) = delete;

template <typename T>
void draw(const T&) = delete;

// CAUTION: The following concept does not work with Apple clang 14.0.0.
template <typename T>
concept IsShape = requires(T t) {
  serialize(t);
  draw(t);
};

class Shape {
  friend void serialize(const Shape& shape) {
    shape.pimpl_->serialize();
  }

  friend void draw(const Shape& shape) {
    std::cout << "draw(const Shape& shape)\n";
    shape.pimpl_->draw();
  }

  friend std::ostream& operator<<(std::ostream& os, const Shape& shape) {
    return os << *shape.pimpl_;
  }

  // The External Polymorphism Design Pattern
  class ShapeConcept {
   public:
    virtual ~ShapeConcept() {}
    virtual void serialize() const = 0;
    virtual void draw() const = 0;
    virtual void print(std::ostream& os) const = 0;

    friend std::ostream& operator<<(
        std::ostream& os, const ShapeConcept& shape) {
      shape.print(os);
      return os;
    }

    // The Prototype Design Pattern
    virtual std::unique_ptr<ShapeConcept> clone() const = 0;
  };


  template <typename T>
  class ShapeModel : public ShapeConcept {
    T object_;

   public:
    ShapeModel(const T& value)
        : object_{value} {
    }

    void serialize() const override {
      // CAUTION: The using declaration tells the compiler to look up the free
      // serialize() function rather than the member function.
      using ::serialize;

      serialize(object_);
    }

    void draw() const override {
      // CAUTION: The using declaration tells the compiler to look up the free
      // draw() function rather than the member function.
      using ::draw;

      draw(object_);
    }

    void print(std::ostream& os) const override {
      os << object_;
    }

    // The Prototype Design Pattern
    std::unique_ptr<ShapeConcept> clone() const override {
      return std::make_unique<ShapeModel>(*this);
    }
  };

  // The Bridge Design Pattern
  std::unique_ptr<ShapeConcept> pimpl_;

 public:
  // CAUTION: `template <IsShape T>` does not work with Apple clang 14.0.0.
  template <typename T>
  Shape(const T& x)
      : pimpl_{new ShapeModel<T>(x)} {
  }

  Shape(const Shape& s)
      : pimpl_{s.pimpl_->clone()} {
  }

  Shape(Shape&& s)
      : pimpl_{std::move(s.pimpl_)} {
  }

  Shape& operator=(const Shape& s) {
    pimpl_ = s.pimpl_->clone();
    return *this;
  }

  Shape& operator=(Shape&& s) {
    pimpl_ = std::move(s.pimpl_);
    return *this;
  }
};

class Circle {
  double radius_;

public:
  explicit Circle(double radius) : radius_(radius) {
  }

  double radius() const {
    return radius_;
  }
};

std::ostream& operator<<(std::ostream& os, const Circle& circle) {
  return os << "Circle(radius = " << circle.radius() << ")";
}

class Square {
  double width_;

public:
  explicit Square(double width) : width_(width) {
  }

  double width() const {
    return width_;
  }
};

std::ostream& operator<<(std::ostream& os, const Square& square) {
  return os << "Square(width = " << square.width() << ")";
}

void serialize(const Circle& circle) {
  std::cout << "Serializing a Circle: " << circle << std::endl;
}

void draw(const Circle& circle) {
  std::cout << "Drawing a Circle: " << circle << std::endl;
}

void serialize(const Square& square) {
  std::cout << "Serializing a Square: " << square << std::endl;
}

void draw(const Square& square) {
  std::cout << "Drawing a Square: " << square << std::endl;
}

int main() {
  using Shapes = std::vector<Shape>;

  Shapes shapes;
  shapes.emplace_back(Circle{2.0});
  shapes.emplace_back(Square{1.5});
  shapes.emplace_back(Circle{4.2});

  std::cout << "\nDrawing all shapes:\n";
  std::for_each(shapes.begin(), shapes.end(), [](const Shape& shape) {
    draw(shape);
  });

  std::cout << "\nSerializing all shapes:\n";
  std::for_each(shapes.begin(), shapes.end(), [](const Shape& shape) {
    serialize(shape);
  });
}
