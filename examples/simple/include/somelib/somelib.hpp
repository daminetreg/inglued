#ifndef SOMELIB_SOMELIB_HPP
#define SOMELIB_SOMELIB_HPP

#include <somelib/inclusive.hpp>

#include <iostream>
#include INCLUSIVE(boost_preprocessor,boost/preprocessor/stringize.hpp)
#include INCLUSIVE(boost_preprocessor,boost/preprocessor/repeat.hpp)
#include INCLUSIVE(boost_preprocessor,boost/preprocessor/cat.hpp)

namespace somelib {
  inline void hello_world() {
    std::cout << BOOST_PP_STRINGIZE(HELLO World!!! BOOST_PP_CAT(cool,_fool)) << std::endl;
  }
}

#endif
