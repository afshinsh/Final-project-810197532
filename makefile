CC := g++

all: System.out

System.out: main.o manager.o customer.o publisher.o comment.o film.o
	$(CC) main.o manager.o customer.o publisher.o comment.o film.o -o System.out

main.o : main.cpp manager.h
	$(CC) -c main.cpp

manager.o: manager.cpp manager.h customer.h comment.h exceptions.h  publisher.h
	$(CC) -c manager.cpp

customer.o: customer.cpp customer.h  film.h customer.h
	$(CC) -c customer.cpp

publisher.o: publisher.cpp publisher.h customer.h 
	$(CC) -c publisher.cpp

comment.o: comment.cpp comment.h 
	$(CC) -c comment.cpp

film.o: film.cpp film.h customer.h comment.h
	$(CC) -c film.cpp

.PHONY: clean
clean: 
	rm *.o
	