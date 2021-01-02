CC=x86_64-w64-mingw32-gcc.exe
CFLAGS=-g -std=c99
LDLIBS=-lSDL2 -lSDL2_image -lSDL2_ttf

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

genetic: genetic.o
	$(CC) $(CFLAGS) genetic.o $(LDLIBS) -o genetic.exe

genetic.o:

risk: risk.o
	$(CC) $(CFLAGS) risk.o $(LDLIBS) -o risk.exe

risk.o:

text: text.o
	$(CC) $(CFLAGS) text.o $(LDLIBS) -o text.exe

text.o:
