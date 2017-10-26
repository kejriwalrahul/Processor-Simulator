all: ProcessorSimulator.o assembler

ProcessorSimulator.o: simulator.cpp
	mkdir -p bin
	g++ -g simulator.cpp -o bin/ProcessorSimulator.o

assembler:
	mkdir -p bin
	javac -d bin/ preprocessing.java

clean:
	rm -f bin/* binary_prog data_dump out