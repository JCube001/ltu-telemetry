# Makefile for AeroC2 CUnit Tests

TARGET = cunit-tests

# Sources files
SOURCES = cunit_tests.c
OBJECTS = $(SOURCES:.c=.o)

# Compiler options
CC = gcc
CPPFLAGS = -I. -I../inc/
CFLAGS = -c -Wall -Wextra
LD = $(CC)
LDFLAGS = -lcunit -lm

# Rules
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

clean:
	@rm -rf $(TARGET) $(OBJECTS)

