CC=gcc
CFLAGS=-Wall -O2
LDFLAGS=

OBJECTS=readlines-main.o readlines.o
TARGET=readlines-main

RM=rm -f

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJECTS) $(TARGET)
