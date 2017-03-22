#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <regex>
#include <boost/format.hpp>
#include <nlohmann/json.hpp>
#include <boost/algorithm/string.hpp>
#include <future>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>
#include <boost/process/system.hpp>

#include <inglued/dep.hpp>
#include <inglued/generate_cmakelists.hpp>


namespace inglued {

  using fmt = boost::format;
  namespace bp = boost::process;
  namespace fs = boost::filesystem;


  //! Reads the list of deps in the from the given  file, and if it exits it's accompanying .transitive.
  inline map_deps_t read_deps(const std::string& path) {
    map_deps_t deps;
    
    auto to_dep = [](nlohmann::json::iterator& it) {
      dep d;
      d.git_uri = it.key();
      d.ref = it.value()["@"].get<std::string>();
      
      if (!it.value()["-I"].is_null()) {
        d.include_path = it.value()["-I"].get<std::string>();
      }

      return d;
    };   

    { // Direct dependencies
      std::ifstream ifs(path.data(), std::ios::in );

      nlohmann::json j;
      ifs >> j;
      for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
        auto d = to_dep(it);
        deps[d.git_uri] = d;
      }
    }

    auto transitive_path = path + ".transitive";
    if (fs::exists(transitive_path)) { // Transitive dependencies
      std::ifstream ifs(transitive_path.data(), std::ios::in );

      nlohmann::json j;
      ifs >> j;
      for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
        auto d = to_dep(it);
        d.transitive = true;
        deps[d.git_uri] = d;
      }
    }

    return deps;
  }

  //! Reads the list of deps in the deps.inglued json file.
  inline void write_transitive_deps(const std::string& path, const map_deps_t& deps) {
    auto to_json = [](auto d) {
      nlohmann::json dep_j;
      dep_j["@"] = d.second.ref;

      if (d.second.include_path != "") {
        dep_j["-I"] = d.second.include_path;
      }

      return dep_j;
    };

    auto transitive_path = (path + ".transitive");
    auto commit_transitive_manifest = [&transitive_path]() {
      bp::system(bp::shell, "git", "add", transitive_path);
      bp::system(bp::shell, "git", "commit", transitive_path, 
        "-m", "\"#glued : transitive dependencies manifest.\"");
    };

    { // Transitive dependencies

      nlohmann::json j;
      for (auto& dep_entry : deps ) {
        if (dep_entry.second.transitive) {
          j[dep_entry.first] = to_json(dep_entry);
        }
      }

      if (j.size() > 0) {
        std::fstream ofs(transitive_path.data(), std::ios::in | std::ios::out | std::ios::trunc );
        ofs << j.dump(2);
        ofs.close();
        commit_transitive_manifest();
      }
    }


  }


  //! Run git to install the dep if not already present
  inline void check_and_clone(const dep& d) {

    std::future<std::string> buffer;

    boost::asio::io_service ios;
    bp::child c(bp::search_path("git"), std::string("subtree")
      , (fs::exists(fs::path("deps") / d.get_name()) ? "pull" : "add")
      , std::string("--prefix=") + "deps/" + d.get_name()
      , d.get_uri()
      , d.ref
      , "--squash"
      , ios
      , (bp::std_out & bp::std_err) > buffer);

    ios.run();
    c.wait();

    if (c.exit_code() != 0) {
      throw std::runtime_error(
        std::string("Cannot add the dependency, your working tree must not have "
        "changes to already commited files. Output is : \n ") + buffer.get());
    }
  }

  //! Delves in each dependencies at 1 depth level and lookup for further includesive deps. If some pull them up here.
  inline void hikeup_deep_deps(map_deps_t& deps) {

    for (auto& d : deps) {
      auto tape = fs::path("deps") / d.second.get_name() / GLUE_PATH;
      if (fs::exists(tape)) {
        auto deep_deps = read_deps(tape.native());

        // For us they are all transitive
        std::for_each(deep_deps.begin(), deep_deps.end(),
          [](auto& p) { p.second.transitive  = true;});

        deps.insert(deep_deps.begin(), deep_deps.end());
      }
    }

    // Write down the transitive deps
    std::cout << "Declaring the transitive dependencies." << std::endl;
    write_transitive_deps(GLUE_PATH, deps);
    std::cout << std::endl;

    std::cout << "Fetching and glueing in your git repository transitive dependencies:" << std::endl;
    // Now that we have hiked up the deps of our deps, clone and add them again at our level.
    // We have to duplicate the files because windows-git-clone symlinks impossible. Sad.
    for (auto& d : deps) {
      if (d.second.transitive) {
        inglued::check_and_clone(d.second); 
        std::cout << "\t" << d.first << " ok." << std::endl;
      } 
    }
    std::cout << std::endl;
  }

 


  //TODO: Inform user of what happenned : present him a tree and it's git changes. Tell him how to revert : glue unseal ?
  //TODO: Generate CONSUME.md with all compilers include path generated to explain how to use.
  

}

  

int main(int argc, const char* argv[]) {
  
  using boost::algorithm::ends_with;

  std::string cmd{(argc > 1) ? argv[1] : "help"};
  if ( (argc <= 1) || ( (argc > 1) && (ends_with(cmd,"help")) ) ) {
    std::cout << "#glued : The header-only dependency manager for library authors.\n";
    std::cout << "\n";

    std::cout << "Simply make a deps/glue file listing your header-only github dependencies, \n"
                 "and glue will commit them to your repository !\n"
                 "\n"
                 "Actually the best way for your users to consume libraries : just clone / download\n"
                 "and all is in the package!\n"
                 "\n"
                 "No git-submodule but good old git-subtree !\n"
                  
                 "Commands : \n"
                 "\t glue seal : Put all dependencies listed in `deps/glue` in your git repository."
                 << std::endl;


  } else if ( cmd == "seal" ) {
    auto deps = inglued::read_deps(inglued::GLUE_PATH);
    
    std::cout << "Fetching and glueing in your git repository direct dependencies:" << std::endl;
    for (auto& d : deps) {
      if (!d.second.transitive) {
        inglued::check_and_clone(d.second); 
        std::cout << "\t" << d.first << " ok." << std::endl;
      }
    }
    std::cout << std::endl;

    hikeup_deep_deps(deps);

  } else if ( (argc > 2) && (cmd == "cmake") ) {
    auto deps = inglued::read_deps(inglued::GLUE_PATH);
    inglued::generate_cmakelists(argv[2], deps);
  } else if (cmd == "cmaketpl") {
    inglued::generate_cmakelists_tpl();
  }

  return 0;
}
