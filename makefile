CC = g++

.SILENT:

all: solver
	./solver
	rm -f *.o

solver: solver.o instance_reader.o
	$(CC) -o solver solver.o instance_reader.o

solver.o: solver.cpp
	$(CC) -c solver.cpp

instance_reader.o: instance_reader.cpp
	$(CC) -c instance_reader.cpp