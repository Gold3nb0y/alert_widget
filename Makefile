#define a series of objects in an array
OBJS := main.o build/alert_box.o
CC = clang
CFLAGS = -g -Wall -I./include/
LIBS = -lX11


all: $(OBJS)# all requires the object files to run
	$(CC) -o alert_widget $(OBJS) $(LIBS)

#the requires part is important for checking the timestamp
main.o : main.c 
	$(CC) -c $(CFLAGS) main.c -o main.o

build/%.o : src/%.c include/%.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm alert_widget main.o build/*
