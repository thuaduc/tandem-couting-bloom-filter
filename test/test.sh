cd ../src;
mkdir -p build;
cd build;
cmake ..;
make;
cd ../../test;
clang++ -std=c++20 -o test -Wall -Wextra -I include/doctest test.cpp -L ../src/build;
./test;