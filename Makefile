.SUFFIXES : .c .o

CC = icc
CFLAGS = -std=c99 -O2

OBJS = main.o rand.o calc.o init.o ctmc.o debug.o tps.o
TARGET = main

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

clean :
	rm $(OBJS) $(TARGET)
