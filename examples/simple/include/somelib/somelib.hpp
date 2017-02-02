#ifndef SOMELIB_SOMELIB_HPP
#define SOMELIB_SOMELIB_HPP

#include <somelib/inclusive.hpp>

#include <iostream>
#include INCLUSIVE(boost_preprocessor,boost/preprocessor/stringize.hpp)

namespace somelib {
  inline void hello_world() {
    std::cout << BOOST_PP_STRINGIZE(HELLO World!!!) << std::endl;
  }
}

#endif
