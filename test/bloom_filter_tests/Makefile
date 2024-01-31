CXX = clang++ -std=c++20
CFLAGS = -Wall -Wextra

ROOT = ../../
SRCDIR = $(ROOT)src/
INCDIR = $(ROOT)include/
OBJDIR = $(ROOT)obj/
BINDIR = $(ROOT)bin/

BLOOM_INCDIR = $(addprefix -I, $(INCDIR)bloom_filter $(SRCDIR)bloom_filter)

ifndef DOCTEST_ABS_DIR
    DOCTEST_ABS_DIR = ../doctest #Default Path for Doctest
endif

ifndef PERFEVENT_ABS_DIR
    PERFEVENT_ABS_DIR = ../ #Default Path for Perfevent
endif 

all: test

test: bloom.a test.o
	$(CXX) $(CFLAGS) $(OBJDIR)test.o -o $(BINDIR)test_bloom $(BINDIR)bloom.a

bloom.a:
	$(MAKE) -C $(ROOT) bloom.a

test.o: test.cpp
	$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)$@ -I$(DOCTEST_ABS_DIR) -I$(PERFEVENT_ABS_DIR) $(BLOOM_INCDIR)

clean:
	rm -f $(OBJDIR)* $(BINDIR)*