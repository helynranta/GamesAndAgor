CC = gcc
CFLAGS = -Wall -pedantic -std=c99
LDFLAGS = -lm
TARGET = test
SOURCE = test.c server_functions.c
OBJECTS = $(SOURCE:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

all:$(TARGET)

remake:
	make clean && make
.PHONY: clean
clean:
	rm -f *~ *.o test
