CC=gcc
TARGET=shell

all:
	$(CC) main.c -o $(TARGET) -lpthread
	
clean:
	rm $(TARGET)
	
