CC = gcc 
CFLAGS = -Wall
PROG_NAME = ftp-get

SRCDIR = src

SOURCES := $(shell find $(SRCDIR) -name '*.c')
OBJECTS := $(SOURCES:%.c=%.o)

.PHONY: clean virtual

all: application

application: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROG_NAME) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(PROG_NAME)