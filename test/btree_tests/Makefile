all:main

btree.a: .FORCE
	$(MAKE) -C src/btree

main: test_main.cpp btree.a tester.hpp
	clang++ -std=c++20 -o $@ -Wall -Wextra $< bin/btree.a -I include -I src/btree -I include/btree

format:
	find . -type f -name '*.?pp' -exec clang-format -i {} \;

.PHONY: .FORCE format
.FORCE:
