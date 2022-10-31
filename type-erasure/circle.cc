#include "type-erasure/circle.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Circle& circle) {
  return os << "Circle(radius = " << circle.radius() << ")";
}

void serialize(const Circle& circle) {
  std::cout << "Serializing a Circle: " << circle << std::endl;
}

void draw(const Circle& circle) {
  std::cout << "Drawing a Circle: " << circle << std::endl;
}

