#define a series of objects in an array
OBJS := main.o
CC = clang
CFLAGS = -g -Wall -I./include/


all: $(OBJS)# all requires the object files to run
	$(CC) -o alert_widget $(OBJS) 

#the requires part is important for checking the timestamp
main.o : main.c 
	$(CC) -c $(CFLAGS) main.c -o main.o

#build/%.o : src/%.c include/%.h
#	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm alert_widget main.o
