#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <boost/format.hpp>
#include <nlohmann/json.hpp>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>

#include <boost/process/system.hpp>


namespace inclusive {

  using fmt = boost::format;
  namespace bp = boost::process;
  namespace fs = boost::filesystem;


  struct dep {
    //! The github path or a git clone uri.
    std::string git_uri;

    //! Git tag or commit ref.
    std::string ref;

    //! The subrepository path to add to include path.
    std::string include_path;

    //! \return The gihub clone URI if simple-single-slash path otherwise git_uri as-is.
    std::string get_uri() const {
      std::regex github_path("[^/]+/[^/]+");
 
      if (std::regex_match(git_uri, github_path)) {
        return std::string("https://github.com/") + git_uri + ".git";
      } else {
        return git_uri;
      }
    }

    std::string get_name() const {
      std::regex only_name("[^/]+/([^/]+)(\\.git)?");
      std::smatch matched;
      std::regex_match(git_uri, matched, only_name);
      
      if (matched.size() < 2) {
        throw std::runtime_error(str(fmt("Error \"%1%\" is an invalid repository URI or github-path !") % git_uri));
      }

      return matched[1];
    }

  };

  //! Reads the list of deps in the deps.inclusive json file.
  inline std::vector<dep> read_deps(const std::string&& path) {
    std::ifstream ifs(path.data(), std::ios::in );

    nlohmann::json j;
    ifs >> j;

    std::vector<dep> deps;
    for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
      dep d;
      d.git_uri = it.key();
      d.ref = it.value()["@"].get<std::string>();
      
      if (!it.value()["?"].is_null()) {
        d.include_path = it.value()["?"].get<std::string>();
      }

      deps.push_back(d);
    }

    return deps;
  }


  //! TODO: run git to install the dep if not already present
  inline void check_and_clone(const dep& d) {

    bp::system(bp::shell, "git", "subtree", (fs::exists(d.get_name()) ? "pull" : "add"),
      "--prefix", d.get_name(),
      d.get_uri(), d.ref,
      "--squash");

  }

  //TODO: Generate CMakeLists.txt to include the stuffs
  //TODO: Generate CONSUME.md with all compilers include path generated to explain how to use.

}

  

int main() {
  auto deps = inclusive::read_deps(".tape"); 

  for (auto& d : deps) { inclusive::check_and_clone(d); }

  return 0;
}
