all: ProcessorSimulator.o assembler

ProcessorSimulator.o: ProcessorSimulator.cpp
	g++ -g ProcessorSimulator.cpp -o bin/ProcessorSimulator.o

assembler:
	javac -d bin/ preprocessing.java

clean:
	rm -f bin/* binary_prog data_dump out