cc_binary(
    name = "type-erasure",
    srcs = [
        "type-erasure.cc",
    ],
)

cc_library(
    name = "shape",
    hdrs = [
        "shape.h",
    ],
)

cc_library(
    name = "circle",
    hdrs = [
        "circle.h",
    ],
    srcs = [
        "circle.cc",
    ],
    deps = [
        ":shape",
    ],
)

cc_library(
    name = "square",
    hdrs = [
        "square.h",
    ],
    srcs = [
        "square.cc",
    ],
    deps = [
        ":shape",
    ],
)

cc_binary(
    name = "main",
    srcs = [
        "main.cc",
    ],
    deps = [
      ":circle",
      ":shape",
      ":square",
    ],
)
