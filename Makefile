CC=x86_64-w64-mingw32-gcc.exe
CFLAGS=-g
LDLIBS=-lSDL2 -lSDL2_image

all: hello perpixel gravity rotation subpixel

hello: hello.o
	$(CC) $(CFLAGS) hello.o $(LDLIBS) -o hello.exe

hello.o:

perpixel: perpixel.o
	$(CC) $(CFLAGS) perpixel.o $(LDLIBS) -o perpixel.exe

perpixel.o:

gravity: gravity.o
	$(CC) $(CFLAGS) gravity.o $(LDLIBS) -o gravity.exe

gravity.o:

rotation: rotation.o
	$(CC) $(CFLAGS) rotation.o $(LDLIBS) -o rotation.exe

rotation.o:

subpixel: subpixel.o
	$(CC) $(CFLAGS) subpixel.o $(LDLIBS) -o subpixel.exe

subpixel.o:

random: random.o
	$(CC) $(CFLAGS) random.o $(LDLIBS) -o random.exe

random.o:
