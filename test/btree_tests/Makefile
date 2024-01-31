CXX = clang++ -std=c++20
CFLAGS = -Wall -Wextra

ROOT = ../../
SRCDIR = $(ROOT)src/
INCDIR = $(ROOT)include/
OBJDIR = $(ROOT)obj/
BINDIR = $(ROOT)bin/

BETREE_INCDIR = $(addprefix -I, $(INCDIR)btree $(SRCDIR)btree)
BLOOM_INCDIR = $(addprefix -I, $(INCDIR)bloom_filter $(SRCDIR)bloom_filter)

ifndef PERFEVENT_ABS_DIR
    PERFEVENT_ABS_DIR = ../ #Default Path for Perfevent
endif 

all: test_btree

test_btree: test_btree.o btree.a tester.hpp
	$(CXX) $(CFLAGS) $(OBJDIR)$< -o $(BINDIR)$@ $(BINDIR)btree.a

btree.a:
	$(MAKE) -C $(ROOT) btree.a

test_btree.o: test_main.cpp
	$(CXX) $(CFLAGS) -c $^ -o $(OBJDIR)$@ -O2 -I$(PERFEVENT_ABS_DIR) $(BETREE_INCDIR) $(BLOOM_INCDIR)

clean:
	rm -f $(OBJDIR)* $(BINDIR)*


