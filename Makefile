CXX = clang++
CFLAGS = -Wall -Wextra -Wpedantic -std=c++20

all:main

btree/btree.a: .FORCE
	cd src/btree; make btree.a;

btree/btree-optimized.a: .FORCE
	cd src/btree; make btree-optimized.a

main: test_main.cpp btree/btree.a tester.hpp PerfEvent.hpp
	$(CXX) $(CFLAGS) -o $@ $< src/btree/btree.a -O2

main-optimized: test_main.cpp btree/btree-optimized.a tester.hpp PerfEvent.hpp
	$(CXX) $(CFLAGS) -o $@ $< src/btree/btree-optimized.a -O3 -DNDEBUG

format:
	find . -type f -name '*.?pp' -exec clang-format -i {} \;

.PHONY: .FORCE format
.FORCE:

