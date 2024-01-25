CXX = clang++ -std=c++20
BINDIR = ../bin/
OBJDIR = ../obj/
INCDIR = ../include/ ../src/ .
DOCDIR = doctest/

CFLAGS = $(foreach D, $(INCDIR), -I$(D)) -I$(DOCDIR) -g -fsanitize=address -fsanitize=undefined -fsanitize=leak

all: test

test: bloom.a test.o
	$(CXX) $(CFLAGS) $(OBJDIR)test.o -o $(BINDIR)test $(BINDIR)bloom.a

bloom.a:
	cd ..; make all

test.o: test.cpp
	$(CXX) $(CFLAGS) -c $< -o $(OBJDIR)$@

clean:
	rm -rf $(OBJDIR)* $(BINDIR)*