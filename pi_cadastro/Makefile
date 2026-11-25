CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c99 -Iinclude
SRC     = $(wildcard src/*.c)

cadastro: $(SRC) $(wildcard include/*.h)
	$(CC) $(CFLAGS) -o cadastro $(SRC)

clean:
	rm -f cadastro cadastro.exe

.PHONY: clean
