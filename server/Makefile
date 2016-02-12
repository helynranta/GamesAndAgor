CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -ggdb
LDFLAGS = -lm -lrt
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
	rm -f *~ *.o $(TARGET)
