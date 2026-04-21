CC = gcc
CFLAGS = -Wall
LIBS = -lfreeglut -lopengl32 -lglu32 -lm

# Arquivos fonte
SRCS = main.c camera.c obj.c textura.c image.c
OBJS = $(SRCS:.c=.o)
TARGET = cinema.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del *.o $(TARGET)

run: $(TARGET)
	$(TARGET)


compilar:
	gcc -o cinema.exe main.c camera.c obj.c textura.c image.c -lfreeglut -lopengl32 -lglu32 -lm -Wall