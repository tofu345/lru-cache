CFLAGS = -g -Wall -Wextra -Werror
DEST = ./build

OBJS = $(DEST)/dll.o $(DEST)/ht.o

tests: tests.c $(OBJS)
	cc ${CFLAGS} -o $(DEST)/$@ $(OBJS) tests.c
	@ valgrind --leak-check=full $(DEST)/$@

$(OBJS): $(DEST)/%.o: %.c %.h
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEST)/tests
