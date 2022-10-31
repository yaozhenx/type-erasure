// Type Erasure Sample Code.
//
// Based on "Breaking Dependencies: Type Erasure - A Design Analysis"
// by Klaus Iglberger, CppCon 2021.
//
// References:
// - https://meetingcpp.com/mcpp/slides/2021/Type%20Erasure%20-%20A%20Design%20Analysis9268.pdf
// - https://www.youtube.com/watch?v=4eeESJQk-mw

#ifndef TYPE_ERASURE_SHAPE_H_
#define TYPE_ERASURE_SHAPE_H_

#include <iostream>
#include <memory>
#include <vector>

// High Level Summary of the Design
// - class Shape and global functions (serialize(), draw(), etc.)
//   - The external client facing interface.
//   - Holds a pointer to ShapeConcept.
// - class ShapeConcept
//   - The internal interface of the Bridge Design Pattern.
//   - It is needed to hide the template parameter of ShapeMode<T>.
// - class ShapeModel<T>
//   - The templated implementation of ShapeConcept.
//   - Routes virtual functions to global functions.
//
// CAUTION: The following forward declarations prevent the compiler from
// complaining about missing global functions serialize() and draw() when
// seeing the using declarations in ShapeModel::serialize() and
// ShapeModel::draw(), as if the compiler does not see the `friend` definitions
// within `class Shape`.
class Shape;
void serialize(const Shape& shape);
void draw(const Shape& shape);

class Shape {
  friend void serialize(const Shape& shape) {
    shape.pimpl_->serialize();
  }

  friend void draw(const Shape& shape) {
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

    // The Prototype Design Pattern
    virtual std::unique_ptr<ShapeConcept> clone() const = 0;

    friend std::ostream& operator<<(
        std::ostream& os, const ShapeConcept& shape) {
      shape.print(os);
      return os;
    }
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
  // A constructor template to create a bridge.
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

#endif  // TYPE_ERASURE_SHAPE_H_

