#ifndef INGLUED_ADAPTER_BOOSTORG_HPP
#define INGLUED_ADAPTER_BOOSTORG_HPP

#include <array>
#include <string>
#include <fstream>
#include <regex>
#include <inglued/dep.hpp>

#include <boost/filesystem.hpp>

/* header-only/adapter.git : 
 
 To find deps of fusion in boost: find . -type f | while read line; do cat $line | grep -P 'boost/[^(fusion)]'; done | sed 's;.*<boost/([^/]+).*;\1;' -r | sort | uniq
 This is a rule definition format id to genericize this code, helped by github-search api.
   
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

  constexpr auto BOOST_LIBRARIES = {
    "accumulators",
    "algorithm",
    "align",
    "any",
    "array",
    "asio",
    "assert",
    "assign",
    "atomic",
    "bimap",
    "bind",
    "chrono",
    "circular_buffer",
    "compatibility",
    "compute",
    "concept_check",
    "config",
    "container",
    "context",
    "conversion",
    "convert",
    "core",
    "coroutine",
    "coroutine2",
    "crc",
    "date_time",
    "detail",
    "disjoint_sets",
    "dll",
    "dynamic_bitset",
    "endian",
    "exception",
    "fiber",
    "filesystem",
    "flyweight",
    "foreach",
    "format",
    "function",
    "function_types",
    "functional",
    "fusion",
    "geometry",
    "gil",
    "graph",
    "graph_parallel",
    "hana",
    "heap",
    "icl",
    "integer",
    "interprocess",
    "intrusive",
    "io",
    "iostreams",
    "iterator",
    "lambda",
    "lexical_cast",
    "local_function",
    "locale",
    "lockfree",
    "log",
    "logic",
    "math",
    "metaparse",
    "move",
    "mpi",
    "mpl",
    "msm",
    "multi_array",
    "multi_index",
    "multiprecision",
    "numeric",
    "optional",
    "parameter",
    "phoenix",
    "polygon",
    "pool",
    "predef",
    "preprocessor",
    "program_options",
    "property_map",
    "property_tree",
    "proto",
    "ptr_container",
    "python",
    "qvm",
    "random",
    "range",
    "ratio",
    "rational",
    "regex",
    "scope_exit",
    "serialization",
    "signals",
    "signals2",
    "smart_ptr",
    "sort",
    "spirit",
    "statechart",
    "static_assert",
    "system",
    "test",
    "thread",
    "throw_exception",
    "timer",
    "tokenizer",
    "tr1",
    "tti",
    "tuple",
    "type_erasure",
    "type_index",
    "type_traits",
    "typeof",
    "units",
    "unordered",
    "utility",
    "uuid",
    "variant",
    "vmd",
    "wave",
    "winapi",
    "xpressive"
  }; 


  //! \brief Detects any boost/ inclusion and add it as an #inglued dep, by finding out the dependencies of it.
  //! \return The deps needed by this boostorg library.
  inline map_deps_t boostorg(const dep& d) {
    auto includes_to_scan = fs::path{"deps"} / d.get_name() / d.include_path;

    std::cout << "########### SCAN OF " << d.get_name() << std::endl; 

    map_deps_t boost_deps{};

    auto end = fs::recursive_directory_iterator{};

    for (fs::directory_entry& entry : fs::recursive_directory_iterator{includes_to_scan}) {

      if ( fs::is_regular_file(entry.path()) ) {
        //std::cout << "scanning " << entry.path().native() << std::endl; 
        std::ifstream ifs(entry.path().native().data());
        ifs.exceptions(std::ios::badbit);
        std::string header;
        ifs.seekg(0, std::ios::end);
        header.resize(ifs.tellg());
        ifs.seekg(0, std::ios::beg);
        ifs.read(const_cast<char*>(header.data()), header.size());

        //std::cout << "File size : " << header.size() << std::endl;

        std::regex include_directive("# *include[^<\"]*(<|\")boost/([^>\"]+)(>|\")");
        auto includes_begin = 
            std::sregex_iterator(header.begin(), header.end(), include_directive);
        auto includes_end = std::sregex_iterator();
     
       // std::cout << "Found "
       //           << std::distance(includes_begin, includes_end)
       //           << " includes.\n";
     
        for (std::sregex_iterator it = includes_begin; it != includes_end; ++it) {
            std::smatch match = *it;
            std::string match_str = match.str();
            //std::cout << "  " << match_str << '\n';

            std::regex other_lib("boost/([^/]+)/.*");
            std::regex core_or_compound("boost/([^\\.]+)\\.");
            std::smatch matched;
            if (std::regex_search(match_str, matched, other_lib)) {
              //// std::cout << " dependency on " << matched[1] << '\n';
              dep detected_d {
                std::string("boostorg/") + matched[1].str(),
                "master",
                "include/",
                true
              };
              boost_deps[detected_d.git_uri] = detected_d;

            } else if (std::regex_search(match_str, matched, core_or_compound)) {
              //// std::cout << "core_or_compound:: dependency on " << matched[1] << '\n';
              //TODO: Here a web query has to be made to check if it's a core component or some compound header from a boost library, but for the moment xxhr::GET needs inglued and therefore we hack the current boost library list. 
              auto found = std::find(BOOST_LIBRARIES.begin(), BOOST_LIBRARIES.end(), matched[1].str());
              dep detected_d;
              if (found != BOOST_LIBRARIES.end()) { // is a boost lib
                 detected_d = dep {
                  std::string("boostorg/") + matched[1].str(),
                  "master",
                  "include/",
                  true
                };
              } else {
                detected_d = dep {
                  "boostorg/core",
                  "master",
                  "include/",
                  true
                };
              }

              boost_deps[detected_d.git_uri] = detected_d;
            }


        }
  
  
      }

    }

    return boost_deps;
  }

}}

#endif
