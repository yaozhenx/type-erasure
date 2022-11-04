// Utility to generate the combined all-in-one version from
// type-erasure/main.cc.
//
// Usage:
//
// ```bash
// $ bazel run //tools:combine ${PWD}
// ```

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>

namespace fs = std::filesystem;

void parse(const std::string& input,
           std::set<std::string>& system_includes,
           std::set<std::string>& custom_includes,
           std::vector<std::string>& contents) {
  std::cout << "Reading " << input << std::endl;
  std::ifstream is(input);
  std::string include_guard;
  std::regex include_guard_ifndef_regex("#ifndef (\\w+)");
  std::regex include_guard_define_regex("#define (\\w+)");
  std::regex include_guard_endif_regex("#endif  // (\\w+)");
  std::regex system_include_regex("#include <.*>");
  std::regex custom_include_regex("#include \"(.*)\"");
  std::string prev;
  std::string line;
  while (std::getline(is, line)) {
    std::smatch matches;
    if (include_guard.empty() &&
        std::regex_match(line, matches, include_guard_ifndef_regex)) {
      include_guard = matches[1].str();
    } else if (!include_guard.empty() &&
               std::regex_match(line, matches, include_guard_define_regex) &&
               matches[1].str() == include_guard) {
      continue;
    } else if (!include_guard.empty() &&
               std::regex_match(line, matches, include_guard_endif_regex) &&
               matches[1].str() == include_guard) {
      continue;
    } else if (std::regex_match(line, matches, system_include_regex)) {
      system_includes.insert(line);
    } else if (std::regex_match(line, matches, custom_include_regex)) {
      if (custom_includes.insert(line).second) {
        std::string header = matches[1].str();
        parse(header, system_includes, custom_includes, contents);
        if (header.ends_with(".h")) {
          std::string src = header.substr(0, header.length() - 2) + ".cc";
          if (fs::exists(fs::path(src))) {
            parse(src, system_includes, custom_includes, contents);
          }
        }
      }
    } else {
      if (!(prev.empty() && line.empty())) {
        contents.push_back(line);
      }
      prev = line;
    }
  }
}

void combine(const std::string& input, const std::string& output) {
  std::set<std::string> system_includes;
  std::set<std::string> custom_includes;
  std::vector<std::string> contents;
  parse(input, system_includes, custom_includes, contents);
  std::ofstream os(output);
  for (const auto& include : system_includes) {
    os << include << '\n';
  }
  os << '\n';
  for (const auto& content : contents) {
    os << content << '\n';
  }
  std::cout << "Writing " << output << std::endl;
}

bool cd_bazel_workspace(fs::path p) {
  while (!fs::exists(p / "WORKSPACE") &&
         !fs::exists(p / "WORKSPACE.bazel") &&
         p.parent_path() != p) {
    p = p.parent_path();
  }

  if (!fs::exists(p / "WORKSPACE") &&
      !fs::exists(p / "WORKSPACE.bazel")) {
    return false;
  }

  fs::current_path(p);
  return true;
}

int main(int argc, char** argv) {
  std::string cwd = std::getenv("BUILD_WORKING_DIRECTORY");
  if (cwd.empty()) {
    cwd = fs::current_path();
  }
  if (!cd_bazel_workspace(cwd)) {
    std::cerr << "Cannot find Bazel WORKSPACE!" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  combine("type-erasure/main.cc", "type-erasure/type-erasure.cc");
  std::cout << "DONE!" << std::endl;
}
