CFLAGS = -g -Wall -Wextra -Werror

BUILDDIR = ./build

OBJS = $(addprefix $(BUILDDIR)/, dll.o ht.o lru.o)

all: $(BUILDDIR) tests

$(BUILDDIR):
	mkdir $(BUILDDIR)

$(BUILDDIR)/%.o: %.c %.h
	cc $(CFLAGS) -c -o $@ $<

tests: tests.c $(OBJS)
	@ cc ${CFLAGS} -o $(BUILDDIR)/$@ $(OBJS) tests.c
	@ valgrind --leak-check=full $(BUILDDIR)/$@

clean:
	rm -rf build
