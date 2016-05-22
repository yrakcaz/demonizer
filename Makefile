EXE=dem
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99 -pedantic
SRC=src/tools.c src/manage.c src/demonize.c src/main.c
OBJ=$(SRC:.c=.o)
TAR=demonizer
PREFIX=/usr/local
CP=cp

-include makefile.rules

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) $(OBJ) $(EXE) $(TAR).tar.bz2

cleandoc:
	$(RM) doc/html doc/latex doc/refman.pdf

distclean: clean cleandoc
	$(RM) makefile.rules

export:
	git archive HEAD --prefix=$(TAR)/ | bzip2 > $(TAR).tar.bz2

doc:
	doxygen doc/Doxyfile
	$(MAKE) -C doc/latex
	mv doc/latex/refman.pdf doc/

install:
	$(CP) $(EXE) $(PREFIX)/bin

uninstall:
	$(RM) $(PREFIX)/bin/$(EXE)


.PHONY: all clean distclean export doc install uninstall
