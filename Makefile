CXX = g++ -std=c++20
SRCDIR = src/
INCDIR = include/ src/
OBJDIR = obj/
TESTDIR = test/
BINDIR = bin/

CFLAGS = -Wall -Wextra -c $(foreach D, $(INCDIR), -I$(D)) -O3

all: bloom.a

bloom.a: bf.o cbf.o vbf.o tbf.o murmurHash.o
	ar rcs $(BINDIR)bloom.a $(OBJDIR)bf.o $(OBJDIR)cbf.o $(OBJDIR)vbf.o $(OBJDIR)tbf.o $(OBJDIR)murmurHash.o
	
bf.o: $(SRCDIR)bf.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

cbf.o: $(SRCDIR)cbf.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

vbf.o: $(SRCDIR)vbf.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

tbf.o: $(SRCDIR)tbf.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@ 

murmurHash.o: $(SRCDIR)murmurHash.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

test: $(TESTDIR)test.cpp
	cd test; make all

clean:
	rm -f $(OBJDIR)* $(BINDIR)*
