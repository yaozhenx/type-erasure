#include "type-erasure/square.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Square& square) {
  return os << "Square(width = " << square.width() << ")";
}

void serialize(const Square& square) {
  std::cout << "Serializing a Square: " << square << std::endl;
}

void draw(const Square& square) {
  std::cout << "Drawing a Square: " << square << std::endl;
}

