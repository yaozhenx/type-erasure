#ifndef TYPE_ERASURE_SQUARE_H_
#define TYPE_ERASURE_SQUARE_H_

#include <iostream>

class Square {
  double width_;

public:
  explicit Square(double width) : width_(width) {
  }

  double width() const {
    return width_;
  }
};

std::ostream& operator<<(std::ostream& os, const Square& square);

void serialize(const Square& square);

void draw(const Square& square);

#endif  // TYPE_ERASURE_SQUARE_H_
