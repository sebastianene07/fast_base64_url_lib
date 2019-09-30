CC=gcc
CFLAGS=-I. -fPIC -g
LDFLAGS=-shared
LIB_NAME=base_64_url
TARGET_LIB=lib$(LIB_NAME).so
TARGET_ELF=base64_url

all: $(TARGET_LIB) test.o
	$(CC) -o $(TARGET_ELF) test.o -L. -l$(LIB_NAME)

test.o: test.c
	$(CC) -g -c $^

$(TARGET_LIB): base64_encode.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: clean all

clean:
	rm -f *.o $(TARGET_ELF) $(TARGET_LIB)
