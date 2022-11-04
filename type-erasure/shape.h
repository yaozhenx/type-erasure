// Type Erasure Sample Code.
//
// Implementation of Klaus Iglberger's C++ Type Erasure Design Pattern.
//
// References:
// - Breaking Dependencies: Type Erasure - A Design Analysis,
//   by Klaus Iglberger, CppCon 2021.
//   - Video: https://www.youtube.com/watch?v=4eeESJQk-mw
//   - Slides: https://meetingcpp.com/mcpp/slides/2021/Type%20Erasure%20-%20A%20Design%20Analysis9268.pdf

#ifndef TYPE_ERASURE_SHAPE_H_
#define TYPE_ERASURE_SHAPE_H_

#include <concepts>
#include <iostream>
#include <memory>

// High Level Summary of the Design
// - `class Shape` and global functions (`serialize()`, `draw()`, etc.)
//   - The external client facing interface.
//   - Holds a pointer to `ShapeConcept` internally.
// - `class ShapeConcept`
//   - The internal interface of the Bridge Design Pattern.
//   - It is needed to hide the template parameter of `ShapeModel<T>`.
// - `class ShapeModel<T>`
//   - The templated implementation of `ShapeConcept`.
//   - Routes virtual functions to global functions.

// CAUTION: The following deleted functions serve 2 purposes:
// 1. Prevent the compiler from complaining about missing global functions
//    `serialize()` and `draw()` when seeing the using declarations in
//    `ShapeModel::serialize()` and `ShapeModel::draw()`, as if the compiler
//    did not see the `friend` definitions within `class Shape`.
// 2. Prevent runaway recursion in case a concrete `Shape` such as `Circle`
//    does not define a `serialize(const Circle&)` or `draw(const Circle&)`
//    function.
template <typename T>
void serialize(const T&) = delete;

template <typename T>
void draw(const T&) = delete;

#ifdef __clang__

// CAUTION: Workaround for clang.
// The following forward declarations of explicit specialization of
// `serialize()` and `draw()` prevent Clang from complaining about redefintion
// errors.
class Shape;

template <>
void serialize(const Shape& shape);

template <>
void draw(const Shape& shape);

#endif  // __clang__

template <typename T>
concept IsShape = requires(T t) {
  serialize(t);
  draw(t);
  { std::declval<std::ostream&>() << t } -> std::same_as<std::ostream&>;
};

class Shape {
  // NOTE: Definition of the explicit specialization has to appear separately
  // later outside of class `Shape`, otherwise it results in error such as:
  //
  // ```
  // error: defining explicit specialization 'serialize<Shape>' in friend declaration
  // ```
  //
  // Reference: https://en.cppreference.com/w/cpp/language/friend
  friend void serialize<>(const Shape& shape);
  friend void draw<>(const Shape& shape);

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
      //
      // Reference: https://stackoverflow.com/a/32091297/4475887
      using ::serialize;

      serialize(object_);
    }

    void draw() const override {
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
  template <IsShape T>
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

template <>
void serialize(const Shape& shape) {
  shape.pimpl_->serialize();
}

template <>
void draw(const Shape& shape) {
  shape.pimpl_->draw();
}

#endif  // TYPE_ERASURE_SHAPE_H_

