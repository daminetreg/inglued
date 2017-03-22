#ifndef INGLUED_GENERATE_CMAKELISTS_HPP
#define INGLUED_GENERATE_CMAKELISTS_HPP

#include <string>
#include <fstream>
#include <mstch/mstch.hpp>
#include <boost/filesystem.hpp>

#include <inglued/dep.hpp>
#include <inglued/cmakelist_tpl.hpp>
#include <inglued/adapter/boostorg.hpp>

namespace inglued {

  namespace fs = boost::filesystem;

  constexpr auto CMAKELISTS_TPL_PATH = "CMakeLists.txt.tpl";
  constexpr auto CMAKE_PACKAGE_CONFIG_TPL_PATH = "cmake/modules/Config.cmake.in.tpl";
  
  constexpr auto CMAKELISTS_PATH = "CMakeLists.txt";
  constexpr auto CMAKE_PACKAGE_CONFIG_PATH = "cmake/modules/Config.cmake.in";

  //! Generates CMakeLists.txt to make the project usable by cmake projects
  inline void generate_cmakelists(const std::string& project, map_deps_t& deps) {
    using boost::algorithm::ends_with;

    std::string cmakelist_view{cmakelist_tpl};
    std::string package_config_view{cmakelist_package_config_tpl};


    auto use_template_if_exists = [](auto tpl_path, std::string& view) {
      if (fs::exists(tpl_path)) {
        std::cout << "😁 CMake expert : your " << tpl_path 
          << " template is used, remove them if you want default."
          << std::endl;

        std::ifstream ifs{tpl_path};
        ifs.exceptions(std::ios::badbit);
        ifs.seekg(0, std::ios::end);
        view.resize(ifs.tellg());
        ifs.seekg(0);
        ifs.read(const_cast<char*>(view.data()), view.size());
      }
    };

    use_template_if_exists(CMAKELISTS_TPL_PATH, cmakelist_view);
    use_template_if_exists(CMAKE_PACKAGE_CONFIG_TPL_PATH, package_config_view);

    auto generate_cmakelists = [&](auto path) {
      mstch::config::escape = [](const std::string& str) { return str; };

      mstch::array mstch_deps;
      for (auto d : deps) {
        mstch_deps.push_back(
          mstch::map{
            {"name", d.second.get_name()},
            {"ref", d.second.ref},
            {"include_path", d.second.include_path},
            {"include_path_end_backslash", ((d.second.include_path.empty()) ? 
                "" 
              : ((ends_with(d.second.include_path, "/")) ? d.second.include_path : (d.second.include_path + "/")) 
              )
            }
          }
        );
      }

      mstch::map context{
        {"project", project},
        {"project_srcs", project},
        {"deps", mstch_deps}
      };
      
      if (!fs::path(path).parent_path().empty()) { fs::create_directories(fs::path(path).parent_path()); }

      std::fstream cmakelists{path, std::ios::trunc | std::ios::in | std::ios::out };
      cmakelists.exceptions(std::ios::badbit);
      cmakelists << mstch::render(cmakelist_view, context);
    };

    auto generate_package_config = [&](auto path) {
      if (!fs::path(path).parent_path().empty()) { fs::create_directories(fs::path(path).parent_path()); }

      std::fstream pkgconfig{path, std::ios::trunc | std::ios::in | std::ios::out };
      pkgconfig.exceptions(std::ios::badbit);
      pkgconfig << package_config_view;
    };

    generate_cmakelists(CMAKELISTS_PATH);
    generate_package_config(CMAKE_PACKAGE_CONFIG_PATH);
  }

  //! Generate tpl file for the cmake freaks !
  void generate_cmakelists_tpl() {

    std::cout << "😁 CMake expert : generating templates that you can edit in "
      << CMAKELISTS_TPL_PATH << " and " << CMAKE_PACKAGE_CONFIG_PATH
      << std::endl;

    std::fstream cmakelists{CMAKELISTS_TPL_PATH, std::ios::trunc | std::ios::in | std::ios::out };
auto constexpr inline_help = R"(
# This is an #inglued <> template !
#
# The template is processed by http://mustache.github.io/, more info about
# syntax here : http://mustache.github.io/mustache.5.html 
#
# You can access the following variables : 
# * {{project}} : current project name
# * {{project_srcs}} : current project srcs folder.
#
# * {{#deps}} {{/deps}} : all deps, direct and transitive 
#   - {{name}}
#   - {{ref}} : tag or branch wished for the dep
#   - {{include_path}} : the path you specified in deps/glue -I
#   - {{include_path_end_backslash}} : same as above but with a guaranteed end slash.
#
)";

    cmakelists << inline_help << cmakelist_tpl;

    std::fstream pkgconfig{CMAKE_PACKAGE_CONFIG_TPL_PATH, std::ios::trunc | std::ios::in | std::ios::out };
    pkgconfig << cmakelist_package_config_tpl;
  }
  
  /*TODO: Handle the mapping of inglued libraries and their system find_package counterparts.
            * Meaning not all libraries are inglued today (which is sad)
            * So how can we include them for the moment ? Anything in boostorg/ can be #inglued
            * with the official Boost find_package. But do we want this ?
  */

    
  //TODO: Generation of simple test folder for the lib.

}

#endif
