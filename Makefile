#Copyright 2019 Radu-Stefan Minea 314CA

CC=gcc
FLAGS=-Wall -Wextra -std=c99
TARGETS=archiver

build: $(TARGETS)

archiver: archiver.c
	$(CC) $(CFLAGS) archiver.c create.c list.c extract.c tema3.c tema3.h -o archiver

clean:
	rm -f $(TARGETS)
