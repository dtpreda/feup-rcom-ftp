PROJ_NAME=ftp

SRCDIR=src

C_SOURCE=$(wildcard $(SRCDIR)/*.c)
H_SOURCE=$(wildcard $(SRCDIR)/*.h)

OBJ=$(C_SOURCE:.c=.o)

CC=gcc
CC_FLAGS=-Wall

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $< $(CC_FLAGS)

%.o: %.c %.h
	$(CC) -o $@ $< $(CC_FLAGS)

main.o: main.c $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS)

clean: 
	rm -f $(OBJ) $(PROJ_NAME)