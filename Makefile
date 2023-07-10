CC = gcc
CFLAGS  = -g -Wall
COMPONENT = *.c
TARGET = run
all:
	$(CC) $(CFLAGS) $(COMPONENT) -o $(TARGET)

clean:
	del /f /q *.o *.exe