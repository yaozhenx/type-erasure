load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")

load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)

all_link_actions = [
    ACTION_NAMES.cpp_link_executable,
    ACTION_NAMES.cpp_link_dynamic_library,
    ACTION_NAMES.cpp_link_nodeps_dynamic_library,
]

def dirname(p):
    """Returns the dirname of a path.
    The dirname is the portion of `p` up to but not including the file portion
    (i.e., the basename). Any slashes immediately preceding the basename are not
    included, unless omitting them would make the dirname empty.
    Args:
      p: The path whose dirname should be returned.
    Returns:
      The dirname of the path.
    """
    prefix, sep, _ = p.rpartition("/")
    if not prefix:
        return sep
    else:
        # If there are multiple consecutive slashes, strip them all out as Python's
        # os.path.dirname does.
        return prefix.rstrip("/")

def _impl(ctx):
    tool_paths = [
        tool_path(
            name = "gcc",
            path = "/opt/homebrew/bin/gcc-12",
        ),
        tool_path(
            name = "ld",
            path = "/usr/bin/ld",
        ),
        tool_path(
            name = "ar",
            path = (ctx.configuration.default_shell_env["WORKSPACE_DIR"] + "/toolchain/ar") if "WORKSPACE_DIR" in ctx.configuration.default_shell_env else "/usr/bin/ar"
        ),
        tool_path(
            name = "cpp",
            path = "/bin/false",
        ),
        tool_path(
            name = "gcov",
            path = "/opt/homebrew/bin/gcov-12",
        ),
        tool_path(
            name = "nm",
            path = "/opt/homebrew/bin/gcc-nm-12",
        ),
        tool_path(
            name = "objdump",
            path = "/bin/false",
        ),
        tool_path(
            name = "strip",
            path = "/bin/false",
        ),
    ]

    features = [
          feature(
              name = "default_linker_flags",
              enabled = True,
              flag_sets = [
                  flag_set(
                      actions = all_link_actions,
                      flag_groups = ([
                          flag_group(
                              flags = [
                                  "-lstdc++",
                              ],
                          ),
                      ]),
                  ),
              ],
          ),
      ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        features = features,
        cxx_builtin_include_directories = [
          "/opt/homebrew/include/c++/12",
          "/opt/homebrew/include/c++/12/aarch64-apple-darwin21",
          "/opt/homebrew/include/c++/12/backward",
          "/opt/homebrew/lib/gcc/current/gcc/aarch64-apple-darwin21/12/include",
          "/opt/homebrew/lib/gcc/current/gcc/aarch64-apple-darwin21/12/include-fixed",
          "/Library/Developer/CommandLineTools/SDKs/MacOSX12.sdk/usr/include",
          "/Library/Developer/CommandLineTools/SDKs/MacOSX12.sdk/System/Library/Frameworks",
        ],
        toolchain_identifier = "local",
        host_system_name = "local",
        target_system_name = "local",
        target_cpu = "k8",
        target_libc = "unknown",
        compiler = "gcc",
        abi_version = "unknown",
        abi_libc_version = "unknown",
        tool_paths = tool_paths,
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)
