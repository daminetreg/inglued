# `#inclusive` : C++ dependencies made easy.
As a library author you have to ensure your library can be used easily. 

The problem is that your users might get an hard time to consume your C++ library, if you do depend on some else library ! Dependency management is hard in C++ because there are :
  - so much target platform
  - so much build systems (cmake, visual studio, scons, autotools, bjam, waf, gyp,...)
  - so much kind of users

Your library will be consumed by two different kind of users :
  - The app developer that just copy/paste your code in his project
  - The system builder which wants clean sysroot and good transitive CMake package dependency.

# We make the app developer and the system packager happy 
Here is a solution to provide your library easily to your users, **without needing** neither you nor your users to install **yet another tool**.

With **One single line more** your library will be available :

  - Direct Integration of transitive dependencies by copy/paste in user project
  - Integration via installation in sysroot setup of include paths, 

# How does it work ?
It simply provides you with a way to package inside **your github repository** the dependencies you need. 

Users just need to include your folder. The rest is done by `#inclusive`.

## Adding a library in 3 steps
[We only support header only dependencies](doc/rationale/WHY_HEADER_ONLY.md), to add one do the following : 

  1. Add it as git subtree : `git subtree add --prefix examples/simple/include/somelib/inclusive/boost-preprocessor git@github.com:boostorg/preprocessor.git boost-1.62.0 --squash`
  2. Copy/paste the header `inclusive.hpp` & add a `deps.hpp` to list your dependencies :

      ```cpp
      #ifndef SOMELIB_INCLUSIVE_HPP
      #define SOMELIB_INCLUSIVE_HPP
        #define INCLUSIVE_boost_preprocessor inclusive/boost-preprocessor/include
      #endif
      ```

  3. In your code use the dependency as follows: 

      ```cpp
      #include <inclusive>
      #include INCLUSIVE(boost_preprocessor,boost/preprocessor/stringize.hpp)
      ```

Finally you are done ! You can now compile any app using your library with : 
  * `g++ -I/path/to/somelib yourprogram.cpp`
