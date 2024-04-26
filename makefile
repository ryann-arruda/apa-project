CC = g++

.SILENT:

all: solver
	./solver
	rm -f *.o

solver: solver.o instance_reader.o neighborhood_structure.o
	$(CC) -O2 -o solver solver.o instance_reader.o neighborhood_structure.o

solver.o: solver.cpp
	$(CC) -c solver.cpp

instance_reader.o: instance_reader.cpp
	$(CC) -c instance_reader.cpp

neighborhood_structure.o: neighborhood_structure.cpp
	$(CC) -c neighborhood_structure.cpp