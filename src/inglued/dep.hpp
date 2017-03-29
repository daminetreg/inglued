#ifndef INGLUED_DEP_HPP
#define INGLUED_DEP_HPP

#include <string>
#include <map>
#include <regex>
#include <boost/format.hpp>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace inglued {

  using fmt = boost::format;

  constexpr auto GLUE_PATH = "deps/inglued";

  struct dep {
    //! The github path or a git clone uri.
    std::string git_uri;

    //! Git tag or commit ref.
    std::string ref;

    //! The subrepository path to add to include path.
    std::string include_path;

    //! True when dependency is needed because of another dependency project.
    bool transitive{};

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

      std::vector<std::string> tokens;
      boost::split(tokens,git_uri,boost::is_any_of("/"));
      std::cout << "VEC=" << tokens.size()  << " 0" << tokens[0] << " 1" << tokens[1] << std::endl;
      if (tokens.size() < 2) {
        throw std::runtime_error(str(fmt("Error \"%1%\" is an invalid github-path !") % git_uri));
      }

      return std::string(tokens[0]) 
        + "/" + std::string(tokens[1]);

    }

    std::string get_gh_organization() const {
      //TODO: Add support for full git URIs.
      std::regex only_name("([^/]+)/[^/]+");
      std::smatch matched;
      std::regex_match(git_uri, matched, only_name);

      return matched[1];
    }

    std::string get_gh_name() const {
      //TODO: Add support for full git URIs.
      std::regex only_name("[^/]+/([^/]+)");
      std::smatch matched;
      std::regex_match(git_uri, matched, only_name);

      return matched[1];
    }

  };

  using map_deps_t = std::map<std::string, dep>;

}


#endif 
