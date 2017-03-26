# `#inglued <>` CMake Support
The CMake support offers 3 feature : 

  * Generating [CMake Package Config](https://cmake.org/cmake/help/v3.8/module/CMakePackageConfigHelpers.html) so that CMake users can `find_package` your lib
  * **Installation or not** of your library owns depedencies
  * Generating build files for your library test/example code

## Generating CMake Package Config so that CMake users can `find_package` your lib
Running `inglued cmake <your-github-name> <repo-name> <main-namespace-folder>` will generate a `CMakeLists.txt`.

When we take [https://github.com/header-only/example-dependency](https://github.com/header-only/example-dependency) we ran :

`inglued cmake header-only example-dependency example-dependency` which results in a CMakeLists.txt which : 

  - Defines an header-only libary for cmake of the name : `header-only::example-dependency`
  - Which `add_subdirectory(test/)`
  - Add installation of the c++ headers in `deps/` and of `example-dependency/`.
  - Add installation of correctly configured CMake Package Config which forwards dependencies include path.


## Installation or not of your lilbrary owns depedencies
If you run your cmake command without any options : 

  * `cmake . -DINGLUED=OFF && make install` Will result in the installation of only your library in [CMAKE_INSTALL_PREFIX/example-dependency/](https://cmake.org/cmake/help/v3.8/variable/CMAKE_INSTALL_PREFIX.html?highlight=install_prefix#variable:CMAKE_INSTALL_PREFIX) 

```
Install the project...
-- Install configuration: ""
-- Installing: /usr/local/include/example-dependency
-- Installing: /usr/local/include/example-dependency/awesome.hpp
-- Installing: /usr/local/lib/cmake/example-dependency/example-dependencyConfig.cmake
-- Installing: /usr/local/lib/cmake/example-dependency/example-dependencyConfigVersion.cmake
-- Installing: /usr/local/lib/cmake/example-dependency/example-dependencyTargets.cmake
```

  * `cmake . && make install` Installs all dependencies of the library in [CMAKE_INSTALL_PREFIX/example-dependency/deps](https://cmake.org/cmake/help/v3.8/variable/CMAKE_INSTALL_PREFIX.html?highlight=install_prefix#variable:CMAKE_INSTALL_PREFIX)

```
Install the project...
-- Install configuration: ""
-- Up-to-date: /usr/local/include/example-dependency
-- Up-to-date: /usr/local/include/example-dependency/awesome.hpp
-- Installing: /usr/local/include/example-dependency/deps
-- Installing: /usr/local/include/example-dependency/deps/boost
[...]
SNIP
[...]
-- Up-to-date: /usr/local/include/example-dependency/deps
-- Installing: /usr/local/include/example-dependency/deps/json.hpp
-- Installing: /usr/local/include/example-dependency/deps/json.hpp.re2c
-- Up-to-date: /usr/local/lib/cmake/example-dependency/example-dependencyConfig.cmake
-- Up-to-date: /usr/local/lib/cmake/example-dependency/example-dependencyConfigVersion.cmake
-- Installing: /usr/local/lib/cmake/example-dependency/example-dependencyTargets.cmake
```

This way a CMake user of your library can simply do :

```
find_package(example-dependency REQUIRED)
add_executable(app app.cpp)
target_link_libraries(app header-only::example-dependency)
```

Following it's choice during installation `-DINGLUED=OFF` or `-DINGLUED=ON`, **app** will use the headers of his sysroot or the headers in `include/example-dependency/deps`.

## Generating build files for your library test/example code.
When running CMake you can do : `cmake .. -DUNIT_TESTS=ON` which will allow you to compile your test files.

You can in the directory `test/` or any other of your choice by using the `CMakeLists.txt.tpl` feature : `inglued cmaketpl` use your library include paths this way : 

```
add_executable(example_test example_test.cpp)
target_link_libraries(example_test header-only::example-dependency)
```

This forwards all `include_directories` you need to the compilation of example_test :

