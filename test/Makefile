CXX = clang++ -std=c++20

ROOT = ../
SRCDIR = $(ROOT)src/
OBJDIR = $(ROOT)obj/
BINDIR = $(ROOT)bin/

DOCTEST_DIR = doctest/ #Path to doctest
PERFEVENT_DIR = ./ #Path to Pervevent

export DOCTEST_DIR PERFEVENT_DIR  

all: test_bloom

test_bloom:
	$(MAKE) -C bloom_filter_tests/

test_btree:
	$(MAKE) -C btree_tests/
	
clean:
	rm -rf $(OBJDIR)* $(BINDIR)*