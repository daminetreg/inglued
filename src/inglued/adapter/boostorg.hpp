#ifndef INGLUED_ADAPTER_BOOSTORG_HPP
#define INGLUED_ADAPTER_BOOSTORG_HPP

#include <string>
#include <fstream>
#include <regex>
#include <inglued/dep.hpp>

#include <boost/filesystem.hpp>

/* header-only/adapter.git : 
 
 {
    "boostorg/[^(boost)]+" : {
      "non-inglued" : "find_package(Boost VERSION ...);",
      "inglued-adapter" : {
        "boost/([^/]+)/.*" : "boostorg/\1",
        "boost/([^\.]+)\.hpp" : "boostorg/core",
        "boost/([^\.]+)\.hpp" : "boostorg/\1"

      }

    },
 }

 */

namespace inglued { namespace adapter {

  namespace fs = boost::filesystem;

  //TODO: Acutally can be used for any boostorg dependency on any lib or glue deps.

/*TODO: Handle the generation of dependencies for libraries which are not #inglued. */
// To find deps of fusion in boost: find . -type f | while read line; do cat $line | grep -P 'boost/[^(fusion)]'; done | sed 's;.*<boost/([^/]+).*;\1;' -r | sort | uniq

  //! \brief Adapts any boostorg/ inclusion as an #inglued dep, by finding out the dependencies of it.
  //! \return The deps needed by this boostorg library.
  inline map_deps_t boostorg(const dep& d) {
    auto includes_to_scan = fs::path{"deps"} / d.get_name() / d.include_path;

    map_deps_t boost_deps{};

    auto end = fs::recursive_directory_iterator{};

    for (fs::directory_entry& entry : fs::recursive_directory_iterator{includes_to_scan}) {

      if ( fs::is_regular_file(entry.path()) ) {
        std::ifstream ifs(entry.path().native().data());
        ifs.exceptions(std::ios::badbit);
        std::string header;
        ifs.seekg(0, std::ios::end);
        header.resize(ifs.tellg());
        ifs.seekg(0, std::ios::beg);
        ifs.read(const_cast<char*>(header.data()), header.size());

        std::cout << "File size : " << header.size() << std::endl;

        std::regex include_directive("# *include[^<\"]*(<|\")boost/([^>\"]+)(>|\")");
        auto includes_begin = 
            std::sregex_iterator(header.begin(), header.end(), include_directive);
        auto includes_end = std::sregex_iterator();
     
        std::cout << "Found "
                  << std::distance(includes_begin, includes_end)
                  << " includes.\n";
     
        for (std::sregex_iterator it = includes_begin; it != includes_end; ++it) {
            std::smatch match = *it;
            std::string match_str = match.str();
            std::cout << "  " << match_str << '\n';

            std::regex other_lib("boost/([^/]+)/.*");
            std::regex core_or_compound("boost/([^\\.]+)\\.");
            std::smatch matched;
            if (std::regex_search(match_str, matched, other_lib)) {
              std::cout << " dependency on " << matched[1] << '\n';
              dep d {
                std::string("boostorg/") + matched[1].str(),
                "master",
                "include/",
                true
              };
              boost_deps[d.git_uri] = d;
            } else if (std::regex_search(match_str, matched, core_or_compound)) {
              std::cout << "core_or_compound:: dependency on " << matched[1] << '\n';
              //TODO: Here a web query has to be made to check if it's a core component or some compound header from a boost library.
              dep d {
                std::string("boostorg/") + matched[1].str(),
                "master",
                "include/",
                true
              };
              boost_deps[d.git_uri] = d;
            }


        }
  
  
  //      std::regex only_name("[^/]+/([^/]+)(\\.git)?");
  //      std::smatch matched;
  //      std::regex_match(git_uri, matched, only_name);
    

      }

    }

    return boost_deps;
  }

}}

#endif
