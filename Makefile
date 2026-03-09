FILES = dll.c
CFLAGS = -g -Wall -Wextra -Werror

TEST_BIN = test_dll

${TEST_BIN}: ${FILES} dll.h test_dll.c Makefile
	gcc ${CFLAGS} -o ${TEST_BIN} ${FILES} test_dll.c
	@ valgrind --leak-check=full ./${TEST_BIN}
