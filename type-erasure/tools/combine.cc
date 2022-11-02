#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>

namespace fs = std::filesystem;

void parse(std::istream& is,
           std::set<std::string>& system_includes,
           std::set<std::string>& custom_includes,
           std::vector<std::string>& contents) {
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
        std::string header = "../../" + matches[1].str();
        std::ifstream header_is(header);
        parse(header_is, system_includes, custom_includes, contents);
        if (header.ends_with(".h")) {
          std::string src = header.substr(0, header.length() - 2) + ".cc";
          if (fs::exists(fs::path(src))) {
            std::ifstream src_is(src);
            parse(src_is, system_includes, custom_includes, contents);
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

void combine(std::istream& is, std::ostream& os) {
  std::set<std::string> system_includes;
  std::set<std::string> custom_includes;
  std::vector<std::string> contents;
  parse(is, system_includes, custom_includes, contents);
  for (const auto& include : system_includes) {
    os << include << '\n';
  }
  os << '\n';
  for (const auto& content : contents) {
    os << content << '\n';
  }
}

int main() {
  std::ifstream is("../main.cc");
  std::ofstream os("../type-erasure.cc");
  combine(is, os);
}
