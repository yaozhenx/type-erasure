# type-erasure

Implementation of Klaus Iglberger's C++ Type Erasure Design Pattern.

The code is based on Klaus Iglberger's talk "Breaking Dependencies: Type
Erasure - A Design Analysis" on CppCon 2021.

The code is tested on macOS Ventura with M1 Max CPU.
- Apple clang 14.0.0.
- GCC 12.2.0 on Ubuntu 22.10 on UTM.
- GCC 12.2.0 with Homebrew.

## Usage

### Running with Clang

Running the single-file version:

```bash
bazel run //type-erasure:type-erasure
```

Running the multiple-file version:

```bash
bazal run //type-erasure:main
```

### Running with GCC (Homebrew) on macOS

Running the single-file version:

```bash
bazel run --config=gcc_config //type-erasure:type-erasure
```

Running the multiple-file version:

```bash
bazel run --config=gcc_config --action_env=WORKSPACE_DIR=${PWD} type-erasure/main
```

## References

- Breaking Dependencies: Type Erasure - A Design Analysis - Klaus Iglberger -
  CppCon 2021
  - Video, https://www.youtube.com/watch?v=4eeESJQk-mw
  - Slides, https://cppcon.digital-medium.co.uk/wp-content/uploads/2021/09/Type-Erasure-A-Design-Analysis.pdf
