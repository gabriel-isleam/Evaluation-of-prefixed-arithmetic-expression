build: main.o functions.o header.h
	gcc -Wall main.o functions.o -lm -o tema3

main.o: main.c 
	gcc -c main.c

functions.o: functions.c
	gcc -c functions.c

run:
	./tema3 test.in test.out

clean: 
	rm main.o functions.o tema3 test.out
