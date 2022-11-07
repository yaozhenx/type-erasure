# type-erasure

Implementation of Klaus Iglberger's C++ Type Erasure Design Pattern.

The code is based on Klaus Iglberger's talk "Breaking Dependencies: Type
Erasure - A Design Analysis" on CppCon 2021.

The code is written in C++20 and tested on macOS Ventura with M1 Max CPU.
- Apple clang 14.0.0.
- GCC 12.2.0 on Ubuntu 22.10 on UTM.
- GCC 12.2.0 with Homebrew.

## Intent

Implement subtyping and runtime polymorphism without coupling of the interface
and concrete classes.

## Requirements

1. Non-intrusive (Open-Closed Principle)
  - Concrete classes do not depend on the base class. (They are not required to
    inherit the same base class.)
  - Operations on concrete classes are defined as global functions. (They are
    not required to have virtual functions.)

2. Supports Subtyping and Runtime Polymorphism (Liskov Substitution Principle)
  - Objects of concrete classes can be implicitly converted to objects of the
    base class.
  - Operations are dispatched at runtime to specialized implementations.

## High Level Summary of the Design

Taking the `Shape` hierarchy as an example.

1. `class Shape` and global functions (`serialize()`, `draw()`, etc.)
  - The external client facing interface.
  - Holds a pointer to `ShapeConcept` internally.

2. `class ShapeConcept`
  - The internal interface of the Bridge Design Pattern.
  - It is needed to hide the template parameter of `ShapeModel<T>`.

3. `class ShapeModel<T>`
  - The templated implementation of `ShapeConcept`.
  - Routes virtual functions to global functions.

## Build and Run with Bazel

### Running with native build environment (Clang on macOS or GCC on Linux)

Run the single-file version:

```bash
bazel run //type-erasure:type-erasure
```

Run the multiple-file version:

```bash
bazal run //type-erasure:main
```

### Running with GCC Homebrew on macOS

Run the single-file version:

```bash
bazel run --config=gcc_config //type-erasure:type-erasure
```

Run the multiple-file version:

```bash
bazel run --config=gcc_config --action_env=WORKSPACE_DIR=${PWD} type-erasure/main
```

## Build and Run with Make

Run the multiple-file version:

```bash
make run
```

Run the single-file version:

```bash
make -C type-erasure run-type-erasure
```

## Dependencies

- (optional) Bazel (https://bazel.build/)
  - How to install: https://github.com/bazelbuild/bazelisk

- (optional) Make

## References

- Breaking Dependencies: Type Erasure - A Design Analysis - Klaus Iglberger -
  CppCon 2021
  - Video, https://www.youtube.com/watch?v=4eeESJQk-mw, https://www.youtube.com/watch?v=jKt6A3wnDyI
  - Slides, https://cppcon.digital-medium.co.uk/wp-content/uploads/2021/09/Type-Erasure-A-Design-Analysis.pdf

- Better Code: Runtime Polymorphism - Sean Parent
  - Video, https://www.youtube.com/watch?v=QGcVXgEVMJg
  - Slides, https://sean-parent.stlab.cc/presentations/2017-01-18-runtime-polymorphism/2017-01-18-runtime-polymorphism.pdf

- Kevlin Henney, C++ Report 12(7), July/August 2000, entitled, Valued Conversions.

