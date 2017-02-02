g++ src/inclusive.hpp -M | sed 's/\/usr\/include\/stdc-predef.h//' | sed 's/inclusive.o://' | sed 's/\\//g' | tac | xargs cat > inclusive.hpp
