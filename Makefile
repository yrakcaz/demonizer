EXE=dem
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99 -pedantic
SRC=src/tools.c src/manage.c src/demonize.c src/main.c
OBJ=$(SRC:.c=.o)
TAR=yrakcaz-demonizer
DIR=/usr/bin

-include makefile.rules

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ) $(EXE) $(TAR).tar.bz2

distclean: clean
	rm -f makefile.rules

export:
	git archive HEAD --prefix=$(TAR)/ | bzip2 > $(TAR).tar.bz2

install:
ifeq ($(DIR),/usr/bin)
	sudo cp dem $(DIR)
else
	cp dem $(DIR)
endif

.PHONY: all clean distclean export
