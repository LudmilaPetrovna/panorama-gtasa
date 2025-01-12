CC=gcc
LDFLAGS=-pthread
LDLIBS=-lpng
CFLAGS=-Wno-pointer-to-int-cast -O0 -ggdb3 -D_SIMPLE_DEMO_APP
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:%.c=%.o)
TARGET=Webserver

all: $(TARGET)

win32: TARGET=Webserver.exe
win32: CC=/usr/bin/i686-w64-mingw32-gcc
win32: LDLIBS=-lwsock32
win32: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $^ $(LDLIBS) $(LDFLAGS)

clean:
	rm -fv a.out $(TARGET) $(OBJECTS)
