CC = gcc

# use C11, debugging and warnings flags
define CFLAGS
-std=c11 \
-g3 \
-fsanitize=address \
-Wpedantic \
-Wall \
-Wextra \
-Wformat=2 \
-Wsign-conversion \
-Wswitch-enum \
-Wno-switch-default \
-Wunused \
-Wfloat-equal \
-Wconversion \
-Wpointer-arith \
-Wtype-limits \
-Wcast-qual \
-Wenum-compare \
-Wsizeof-pointer-memaccess \
-Wstrict-prototypes \
`pkg-config --cflags check` \
-Wno-unused-parameter
endef

LDFLAGS = -fsanitize=address

PROG = test

all: $(PROG)

RBTree.o: RBTree.c RBTree.h

test: test.o RBTree.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(PROG)

valgrind: LDFLAGS=-lm
valgrind: CFLAGS=-Wall -g3
valgrind: $(PROG)
