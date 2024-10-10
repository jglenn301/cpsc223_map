CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

BikeCount: bike_count.o simap_array.o string_util.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

simap_array.o: simap.h
bike_count.o: simap.h string_util.h
