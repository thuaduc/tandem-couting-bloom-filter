CXX = clang++ -std=c++20
CFLAGS = -Wall -Wextra -Wpedantic -c

all: btree.a

btree.a: btree.o slotted_page.o utility.o printTree.o
	rm -f btree.a
	ar rcs $@ $^

btree-optimized.a: btree-optimized.o slotted_page-optimized.o utility.o printTree.o
	rm -f btree-optimized.a
	ar rcs $@ $^

btree.o: btree.cpp btree.hpp
	$(CXX) $(CFLAGS) btree.cpp -o $@ -O2

btree-optimized.o: btree.cpp btree.hpp
	$(CXX) $(CFLAGS) btree.cpp -o $@ -O3 -DNDEBUG

slotted_page.o: slotted_page.cpp slotted_page.hpp
	$(CXX) $(CFLAGS) slotted_page.cpp -o $@ -O2

slotted_page-optimized.o: slotted_page.cpp slotted_page.hpp
	$(CXX) $(CFLAGS) slotted_page.cpp -o $@ -O3 -DNDEBUG

utility.o: utility.cpp utility.hpp
	$(CXX) $(CFLAGS) utility.cpp -o $@ -O2

printTree.o: printTree.cpp printTree.hpp
	$(CXX) $(CFLAGS) printTree.cpp -o $@ -O2

clean:
	rm -f btree.o slotted_page.o utility.o printTree.o btree.a

