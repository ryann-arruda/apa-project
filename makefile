CC = g++

.SILENT:

all: solver
	rm -f *.o

solver: solver.o instance_reader.o neighborhood_structure.o metaheuristica.o execution_report.o greedy_algorithm.o local_search.o
	$(CC) -O2 -o solver solver.o instance_reader.o neighborhood_structure.o metaheuristica.o execution_report.o greedy_algorithm.o local_search.o

solver.o: solver.cpp
	$(CC) -c solver.cpp

instance_reader.o: instance_reader.cpp
	$(CC) -c instance_reader.cpp

neighborhood_structure.o: neighborhood_structure.cpp
	$(CC) -c neighborhood_structure.cpp

metaheuristica.o: metaheuristica.cpp
	$(CC) -c metaheuristica.cpp

execution_report.o: execution_report.cpp
	$(CC) -c execution_report.cpp

greedy_algorithm.o: greedy_algorithm.cpp
	$(CC) -c greedy_algorithm.cpp

local_search.o: local_search.cpp
	$(CC) -c local_search.cpp