EXE=dem
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99 -pedantic
SRC=src/demonize.c src/main.c
OBJ=$(SRC:.c=.o)
TAR=yrakcaz-demonizer

-include makefile.rules

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $^

%.oc: %.cc
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ) $(EXE) $(TAR).tar.bz2

distclean: clean
	rm -f makefile.rules

export:
	git archive HEAD --prefix=$(TAR)/ | bzip2 > $(TAR).tar.bz2

.PHONY: all clean distclean export
