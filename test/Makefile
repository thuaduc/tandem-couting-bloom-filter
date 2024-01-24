CXX = clang++ -std=c++20
BINDIR = ../bin/
OBJDIR = ../obj/
INCDIR = ../include/ ../src/ .
DOCDIR = doctest/

CFLAGS = $(foreach D, $(INCDIR), -I$(D)) -I$(DOCDIR)

all: test

test: bloom.a test.o
	$(CXX) $(CFLAGS) $(OBJDIR)test.o -o $(BINDIR)test -L $(BINDIR)bloom.a

bloom.a:
	cd ..; make all

test.o: test.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

clean:
	rm -rf $(OBJDIR)* $(BINDIR)*