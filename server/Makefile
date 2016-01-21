CC = gcc
CFLAGS = -Wall -pedantic -std=c99
LDFLAGS = -lm
TARGET = server
SOURCE = server.c server_functions.c server_unpackers.c
OBJECTS = $(SOURCE:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS) -I.

all:$(TARGET)

remake:
	make clean && make
.PHONY: clean
clean:
	rm -f *~ *.o test
