# type-erasure

Implementation of Klaus Iglberger's C++ Type Erasure Design Pattern.

The code is based on Klaus Iglberger's talk "Breaking Dependencies: Type
Erasure - A Design Analysis" on CppCon 2021.

The code is tested with Apple clang 14.0.0 on macOS Ventura.

It does NOT work with Homebrew GCC 12.2.0 on macOS Ventura.

## Usage

Running the all-in-one version:

```bash
bazel run type-erasure/type-erasure
```

Running the multi-file version:

```bash
bazal run type-erasure/main
```

## References

- Breaking Dependencies: Type Erasure - A Design Analysis - Klaus Iglberger -
  CppCon 2021
  - Video, https://www.youtube.com/watch?v=4eeESJQk-mw
  - Slides, https://cppcon.digital-medium.co.uk/wp-content/uploads/2021/09/Type-Erasure-A-Design-Analysis.pdf
