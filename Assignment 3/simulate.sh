# Script to simulate input assembly given in $1
	
	# Change to binary directory
	cd bin
	# Convert assembly to binary IR
	cp ../$1 input
	java PreProcess > binary_prog
	rm input
	cat binary_prog
	
	# Load binary IR and simulate execution
	# ./ProcessorSimulator.o binary_prog ../data_dump
	# Remove temporary file
	# rm -f binary_prog
	# Output the memory dump
	# cat ../data_dump