CC=gcc
CFLAGS=-Wall -pedantic -std=c99 -g3

BikeCount: bike_count.o simap_chained.o string_util.o string_key.o
	${CC} ${CCFLAGS} -o $@ $^ -lm

simap_chained.o: simap.h
bike_count.o: simap.h string_util.h string_key.h
string_key.o: string_key.h
string_util.o: string_util.h
