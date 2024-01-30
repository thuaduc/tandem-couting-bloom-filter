CXX = clang++ -std=c++20

SRCDIR = src/
OBJDIR = obj/
BINDIR = bin/

OBJS = $(wildcard $(OBJDIR)*.o)

all: bloom.a

bloom.a: btree
	ar rcs $(BINDIR)$@ $(OBJS)

btree:
	$(MAKE) -C $(SRCDIR)btree

clean:
	rm -rf $(OBJDIR)* $(BINDIR)*