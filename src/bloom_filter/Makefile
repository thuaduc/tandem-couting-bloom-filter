CXX = clang++ -std=c++20
ROOTDIR = ../../
INCDIR = $(ROOTDIR)include/bloom_filter/ .
OBJDIR = $(ROOTDIR)obj/
BINDIR = $(ROOTDIR)bin/

CFLAGS = -Wall -Wextra -c $(foreach D, $(INCDIR), -I$(D)) -O3

all: bloom

bloom: bf.o cbf.o vbf.o tbf.o murmurHash.o utility_bloom.o

bf.o: bf.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

cbf.o: cbf.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

vbf.o: vbf.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

tbf.o: tbf.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@ 

murmurHash.o: murmurHash.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

utility_bloom.o: utility.cpp
	$(CXX) $(CFLAGS) $< -o $(OBJDIR)$@

clean:
	rm -f $(OBJDIR)* $(BINDIR)*
