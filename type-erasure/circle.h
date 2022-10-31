#ifndef TYPE_ERASURE_CIRCLE_H_
#define TYPE_ERASURE_CIRCLE_H_

#include <iostream>

class Circle {
  double radius_;

public:
  explicit Circle(double radius) : radius_(radius) {
  }

  double radius() const {
    return radius_;
  }
};

std::ostream& operator<<(std::ostream& os, const Circle& circle);

void serialize(const Circle& circle);

void draw(const Circle& circle);

#endif  // TYPE_ERASURE_CIRCLE_H_
