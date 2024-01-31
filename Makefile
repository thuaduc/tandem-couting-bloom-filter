CXX = clang++ -std=c++20

SRCDIR = src/
OBJDIR = obj/
BINDIR = bin/
TESTDIR = test/

OBJS_ALL = $(wildcard $(OBJDIR)*.o)
OBJS_BLOOM = $(addprefix $(OBJDIR), bf.o cbf.o tbf.o vbf.o utility_bloom.o murmurHash.o)

all: btree.a bloom.a btree_test bloom_test

btree.a: btree
	ar rcs $(BINDIR)$@ $(OBJS_ALL)

btree:
	$(MAKE) -C $(SRCDIR)btree

bloom.a: bloom
	ar rcs $(BINDIR)$@ $(OBJS_BLOOM)

bloom:
	$(MAKE) -C $(SRCDIR)bloom_filter

btree_test:
	$(MAKE) -C $(TESTDIR) test_btree

bloom_test:
	$(MAKE) -C $(TESTDIR) test_bloom

clean:
	rm -rf $(OBJDIR)* $(BINDIR)*