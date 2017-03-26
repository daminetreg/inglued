#ifndef INGLUED_INSTALL_DOC_TPL_HPP
#define INGLUED_INSTALL_DOC_TPL_HPP

namespace inglued {

  //! Markdown template for INSTALL.md
  constexpr auto install_doc_tpl = R"(
# Installing {{org}}::{{project}}
{{project}} can be installed in three ways :

* [Copy paste in your project](#copy-paste-in-your-project)
* [Install](#install)
* [Install without dependencies](#install-without-dependencies)

## Copy paste in your project
1. Copy this folder in your project
2. Add the following to your include path :

### GCC / Clang / MSVC
```
-I {{project}}/ \
{{#deps}}
-I {{project}}/deps/{{org}}/{{name}}/{{include_path}} \
{{/deps}}
```

## Install
Install the library
```sh
mkdir build/ && cd build/
cmake ..
make install
```

You can tweak destination with `-DCMAKE_INSTALL_PREFIX=/install/path` to change the default `/usr/local`.

## If you use CMake :
```cmake
find_package({{project}} REQUIRED)
add_executable(app your_code.cpp)
target_link_libraries(app {{org}}::{{project}})
```

## If you don't use CMake : 
Add to your include path : 
```
INSTALL_PREFIX/include/{{project}}/deps/
INSTALL_PREFIX/include/
```

## Install without dependencies
Install the library
```sh
mkdir build/ && cd build/
cmake .. -DINGLUED=OFF
make install
```

This library was built with [`#inglued <>`](https://github.com/header-only/inglued) therefore it forwards dependencies with it, but you might prefer the use of your own deps, therefore simply disable them via `INGLUED=OFF`.
)";


}

#endif
