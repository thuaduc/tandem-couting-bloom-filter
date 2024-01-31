CXX = clang++ -std=c++20

ROOT = ../
SRCDIR = $(ROOT)src/
OBJDIR = $(ROOT)obj/
BINDIR = $(ROOT)bin/

DOCTEST_DIR = doctest/ #Path to doctest
PERFEVENT_DIR = ./ #Path to Pervevent

DOCTEST_ABS_DIR = $(abspath $(DOCTEST_DIR))
PERFEVENT_ABS_DIR = $(abspath $(PERFEVENT_DIR))

export DOCTEST_ABS_DIR PERFEVENT_ABS_DIR  

all: test_bloom

test_bloom:
	$(MAKE) -C bloom_filter_tests/

test_btree:
	$(MAKE) -C btree_tests/
	
clean:
	rm -rf $(OBJDIR)* $(BINDIR)*