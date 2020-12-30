CC=x86_64-w64-mingw32-gcc.exe
CFLAGS=-g
LDLIBS=-lSDL2 -lSDL2_image
OBJECTS=hello.o

hello: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDLIBS) -o hello.exe

hello.o:

perpixel: perpixel.o
	$(CC) $(CFLAGS) perpixel.o $(LDLIBS) -o perpixel.exe

perpixel.o:

gravity: gravity.o
	$(CC) $(CFLAGS) gravity.o $(LDLIBS) -o gravity.exe

gravity.o:
