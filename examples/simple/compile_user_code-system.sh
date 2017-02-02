#!/bin/bash
echo "g++ -Iinclude/ example_user_code.cpp -o example_user_code -DINCLUSIVE_DISABLED"
g++ -Iinclude/ example_user_code.cpp -o example_user_code -DINCLUSIVE_DISABLED
