run: first_pass.o second_pass.o macro_process.o utils.o main.o
	gcc -g -pedantic -ansi -Wall first_pass.o second_pass.o macro_process.o utils.o main.o -o run
first_pass.o: first_pass.c first_pass.h
	gcc -c -g -pedantic -ansi -Wall first_pass.c -o first_pass.o
second_pass.o: second_pass.c second_pass.h
	gcc -c -g -pedantic -ansi -Wall second_pass.c -o second_pass.o
macro_process.o: macro_process.c macro_process.h
	gcc -c -g -pedantic -ansi -Wall macro_process.c -o macro_process.o
utils.o: utils.c utils.h
	gcc -c -g -pedantic -ansi -Wall utils.c -o utils.o
main.o: main.c main.h utils.h
	gcc -c -g -pedantic -ansi -Wall main.c -o main.o

