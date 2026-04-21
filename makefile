CC = gcc
CFLAGS = -Wall
LIBS = -lfreeglut -lopengl32 -lglu32 -lm

all: cinema

cinema: main.o camera.o obj.o
	$(CC) -o cinema.exe main.o camera.o obj.o $(LIBS)

main.o: main.c camera.h obj.h
	$(CC) $(CFLAGS) -c main.c

camera.o: camera.c camera.h
	$(CC) $(CFLAGS) -c camera.c

obj.o: obj.c obj.h
	$(CC) $(CFLAGS) -c obj.c

clean:
	rm -f *.o cinema.exe

run: cinema.exe
	./cinema.exe