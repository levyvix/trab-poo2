compile:
	g++ src/main.cpp

run: compile
	./a.out data < data/entrada.txt