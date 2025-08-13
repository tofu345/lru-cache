FILES = test_dll.c dll.c
OUTFILE = test_dll
CFLAGS = -g -Wall -Wextra -Werror -o ${OUTFILE}

test_dll: ${FILES} Makefile
	@ gcc ${CFLAGS} ${FILES}
