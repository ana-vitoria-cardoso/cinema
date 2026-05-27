CC = gcc
CFLAGS = -Wall
LIBS = -lfreeglut -lglu32 -lopengl32 -lm

# Arquivos fonte atualizados com animacao.c e tempo.c
SRCS = main.c camera.c obj.c textura.c image.c animacao.c tempo.c
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

# Regra complementar ajustada com todos os arquivos fontes necessários
compilar:
	gcc -o cinema.exe main.c camera.c obj.c textura.c image.c animacao.c tempo.c -lfreeglut -lglu32 -lopengl32 -lm -Wall