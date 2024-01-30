CXX = clang++ -std=c++20
ROOTDIR = ../../
INCDIR = $(ROOTDIR)include/btree/ .
OBJDIR = $(ROOTDIR)obj/
BINDIR = $(ROOTDIR)bin/


CFLAGS = -Wall -Wextra -c $(foreach D, $(INCDIR), -I$(D))

all: btree

btree: btree.o slotted_page.o utility.o bloom;

btree-optimized: btree-optimized.o slotted_page-optimized.o utility.o bloom;

btree.o: btree.cpp
	$(CXX) $(CFLAGS) btree.cpp -o $(OBJDIR)$@ -O2

btree-optimized.o: btree.cpp
	$(CXX) $(CFLAGS) btree.cpp -o $(OBJDIR)$@ -O3 -DNDEBUG

slotted_page.o: slotted_page.cpp
	$(CXX) $(CFLAGS) slotted_page.cpp -o $(OBJDIR)$@ -O2

slotted_page-optimized.o: slotted_page.cpp
	$(CXX) $(CFLAGS) slotted_page.cpp -o $(OBJDIR)$@ -O3 -DNDEBUG

utility.o: utility.cpp
	$(CXX) $(CFLAGS) utility.cpp -o $(OBJDIR)utility_btree.o -O2

bloom:
	$(MAKE) -C ../bloom_filter/

clean:
	rm -f $(OBJDIR)* $(BINDIR)*
