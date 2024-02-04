CXX = clang++ -std=c++20
CFLAGS = -Wall -Wextra -c -O3

SRCDIR = src/
INCDIR = include/
OBJDIR = obj/
BINDIR = bin/
TESTDIR = test/

OBJS = $(addprefix $(OBJDIR), bf.o cbf.o vbf.o tbf.o murmurHash.o utility.o)
INCLUDE_FLAGS = $(addprefix -I, $(INCDIR) $(SRCDIR))

.PHONY: all clean test

all: $(BINDIR)bloom.a

$(BINDIR)bloom.a: $(OBJS)
	ar rcs $@ $^

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CFLAGS) $(INCLUDE_FLAGS) $< -o $@

test:
	$(MAKE) -C $(TESTDIR)

clean:
	rm -rf $(OBJDIR)* $(BINDIR)*

