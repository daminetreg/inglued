#ifndef INGLUED_TPL_HPP
#define INGLUED_TPL_HPP

#include <fstream>
#include <string> 
#include <boost/filesystem.hpp>

namespace inglued {
  
  namespace fs = boost::filesystem;

  inline void use_template_if_exists(fs::path tpl_path, std::string& view) {
    if (fs::exists(tpl_path)) {
      std::cout << "😁 Your " << tpl_path 
        << " template is used, remove it if you want default behaviour."
        << std::endl;

      std::ifstream ifs{tpl_path.native().data()};
      ifs.exceptions(std::ios::badbit);
      ifs.seekg(0, std::ios::end);
      view.resize(ifs.tellg());
      ifs.seekg(0);
      ifs.read(const_cast<char*>(view.data()), view.size());
    }
  };
}

#endif
