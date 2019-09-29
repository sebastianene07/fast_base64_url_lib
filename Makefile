CC=gcc
CFLAGS=-I. -fPIC -g
LDFLAGS=-shared
LIB_NAME=base_64_url
TARGET_LIB=lib$(LIB_NAME).so

all: $(TARGET_LIB) test.o
	$(CC) -o test test.o -L. -l$(LIB_NAME)

test.o: test.c
	$(CC) -c $^

$(TARGET_LIB): base64_encode.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: clean all

clean:
	rm -f *.o test $(TARGET_LIB)
