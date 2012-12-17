CC=gcc
CFLAGS=-Wall -O2

RM=rm -f

all: readlines-main.o readlines.o
	$(CC) -o readlines-main readlines-main.o readlines.o

all-shared: readlines-main.o libreadlines.so
	$(CC) -L. -o readlines-main readlines-main.o libreadlines.so

%.o: %.c
	$(CC) $(CFLAGS) -c $<

libreadlines.so: readlines.o
	$(CC) -shared -o $@ $<

clean:
	$(RM) readlines-main readlines-main.o readlines.o libreadlines.so
