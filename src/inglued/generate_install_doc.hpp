#ifndef INGLUED_GENERATE_INSTALL_DOC_HPP
#define INGLUED_GENERATE_INSTALL_DOC_HPP

#include <boost/filesystem.hpp>
#include <mstch/mstch.hpp>

#include <inglued/install_doc_tpl.hpp>
#include <inglued/dep.hpp>
#include <inglued/tpl.hpp>

namespace inglued {
  
  namespace fs = boost::filesystem;

  constexpr auto INSTALL_DOC_TPL_PATH="INSTALL.md.tpl";
  constexpr auto INSTALL_DOC_PATH="INSTALL.md";

  inline void generate_install_doc(const std::string& org, const std::string& project, const std::string& project_srcs, map_deps_t& deps) {

    std::string install_doc_view{install_doc_tpl};

    use_template_if_exists(INSTALL_DOC_TPL_PATH, install_doc_view);

    mstch::config::escape = [](const std::string& str) { return str; };

    mstch::array mstch_deps;
    for (auto d : deps) {

      mstch_deps.push_back(
        mstch::map{
          {"org", d.second.get_gh_organization()},
          {"name", d.second.get_gh_name()},
          {"ref", d.second.ref},
          {"include_path", d.second.include_path}
        }
      );
    }

    mstch::map context{
      {"org", org},
      {"project", project},
      {"project_srcs", project_srcs},
      {"deps", mstch_deps}
    };
    
    fs::path path{INSTALL_DOC_PATH};
    if (!fs::path(path).parent_path().empty()) { fs::create_directories(fs::path(path).parent_path()); }

    std::fstream install_md{path.native().data(), std::ios::trunc | std::ios::in | std::ios::out };
    install_md.exceptions(std::ios::badbit);
    install_md << mstch::render(install_doc_view, context);
  }
}



#endif
