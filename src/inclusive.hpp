#ifndef INCLUSIVE_HPP
#define INCLUSIVE_HPP

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/expand.hpp>

#ifdef INCLUSIVE_DISABLED
  #define INCLUSIVE(NAMESPACE, HEADER) #HEADER
#else // INCLUSIVE_DISABLED 
  #define INCLUSIVE(NAMESPACE, HEADER) BOOST_PP_STRINGIZE( BOOST_PP_EXPAND(BOOST_PP_CAT(INCLUSIVE_,NAMESPACE))HEADER)
#endif // INCLUSIVE_DISABLED 

#endif

#include "deps.hpp"
