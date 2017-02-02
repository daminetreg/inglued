# \#inclusive : C++ dependencies made easy.
As a library author you have to ensure the library can be used easily. 

The problem is that a library user might get an hard time to consume a C++ library ! Dependency management is hard in C++ because there are :
  - so much target platform
  - so much build systems (cmake, visual studion, scons, autotools, bjam, waf, gyp,...)
  - so much kind of users

Your library will be consumed by two different kind of users :
  - The app developer that just copy/paste your code in his project
  - The system builder which wants clean sysroot and good transitive CMake package dependency.

# We make the app developer and the system packager happy 
Here is a solution to provide your library easily to your users, **without needing** your users to install **yet another tool**.

With **One single line** your library will be available :

  - As CMake dependency (CMake Package Config)
  - Direct Integration of transitive dependencies by copy/paste in user project
  - Integration via installation in sysroot setup of include paths, 

# How does it work ?
It simply generates a concatenated header of all your dependencies based on a **oneliner configuration** using **github** as datasource for dependency inclusion. 

In your project deposit anywhere a .inclusive file (You can hide this to your user):
```json
{
  "this" : "<mylib/?>",
  "deps" : {
    "<boost/preprocessor/?>" : "boostorg/preprocessor.github",
    "<pre/type_traits/?>" : "cpp-pre/type_traits.git",
  }
}
```

## Generate your **shippable / git commitable** dependency : 
It will use the compiler it finds on the path between gcc, clang or msvc :

* `inclusive <path/to/your/headers>`
* or `inclusive <master-unit-test.cpp>`

It does only include the library listed in **.inclusive**, so **std** and other system expected libraries are not.

The header generated will be placed in the folder include path specified by *this* key.
