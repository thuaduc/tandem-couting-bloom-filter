CXX = clang++ -std=c++20
CFLAGS = -Wall -Wextra -O3
DEPFLAGS = -MP -MD

ROOT = ../
SRCDIR = $(ROOT)src/
INCDIR = $(ROOT)include/
OBJDIR = $(ROOT)obj/
BINDIR = $(ROOT)bin/

DOCTEST_DIR = doctest/ #Path to doctest
PERFEVENT_DIR = ./ #Path to Pervevent

INCLUDE_FLAGS = $(addprefix -I, $(INCDIR) $(SRCDIR) $(DOCTEST_DIR) $(PERFEVENT_DIR))
DEPS = $(addprefix $(OBJDIR), test.d)
-include $(DEPS)


.PHONY: all clean

all: $(BINDIR)test

$(BINDIR)test: $(OBJDIR)test.o $(BINDIR)bloom.a
	$(CXX) $(CFLAGS) $< -o $@ $(BINDIR)bloom.a $(INCLUDE_FLAGS)

$(BINDIR)bloom.a:
	$(MAKE) -C $(ROOT)

$(OBJDIR)test.o: test.cpp
	$(CXX) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ $(INCLUDE_FLAGS)

clean:
	rm -rf $(OBJDIR)test.o $(OBJDIR)test $(OBJDIR)test.d 