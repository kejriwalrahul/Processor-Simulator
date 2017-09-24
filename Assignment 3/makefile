all: ProcessorSimulator.o assembler

ProcessorSimulator.o: ProcessorSimulator.cpp
	g++ -g ProcessorSimulator.cpp -o ProcessorSimulator.o

assembler:
	javac preprocessing.java

run: 
	java PreProcess $1 >temp_out
	./ProcessorSimulator.o temp_out final_out
	rm -f temp_out

clean:
	rm -f *.o infile outfile *.class temp_out final_out out