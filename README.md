# type-erasure

Implementation of the C++ Type Erasure Design Pattern.

The code is based on Klaus Iglberger's talk "Breaking Dependencies: Type
Erasure - A Design Analysis" on CppCon 2021.

The code is tested with clang 14.0.0 on macOS Ventura.

// A constructor template to create a bridge.
// NOTE: Make it `explicit` to avoid the runaway recursion when a concrete
// `Shape`, e.g. `Circle`, does not define a global function, e.g.
// `draw(const Circle&)`.
//
// TODO: Define a concept requires all global functions to be defined for T.
template <typename T>
explicit Shape(const T& x)
    : pimpl_{new ShapeModel<T>(x)} {
}

## References

- Breaking Dependencies: Type Erasure - A Design Analysis - Klaus Iglberger -
  CppCon 2021
  - Video, https://www.youtube.com/watch?v=4eeESJQk-mw
  - Slides, https://cppcon.digital-medium.co.uk/wp-content/uploads/2021/09/Type-Erasure-A-Design-Analysis.pdf
