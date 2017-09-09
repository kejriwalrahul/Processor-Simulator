/*
	Program to simulate a processor

	Rahul Kejriwal
	CS14B023

	Abinash Patra
	CS14B032
*/

#include <iostream>

/*
	Size Limits
*/
#define MAX_IMEM_SIZE 256
#define MAX_DMEM_SIZE 512
#define MAX_REGS 16

/*
	Instruction Reference
*/
#define ADD  0
#define ADDI 1
#define SUB  2
#define SUBI 3
#define MUL  6
#define MULI 7

#define LD  8
#define ST 10

#define JMP  12
#define BEQZ 13
#define HLT  14

/*
	Structure to hold instructions
*/
typedef struct {
	unsigned short opcode : 4,
	unsigned short op1 : 4,
	unsigned short op2 : 4,
	unsigned short op3 : 4

	void reset(){
		opcode = 0;
		op1 = op2 = op3 = 0;
	}

} Instruction;

/*
	Structure to hold 10-bit memory addresses
*/
typedef struct {
	unsigned short isData : 1,
	unsigned short address : 9

	void reset(){
		isData = 0;
		address = 0;
	}

} MemoryAddress;

// Typedef-ed
typedef unsigned char  DataElement;
typedef unsigned short Data;

/*
	Simulation Processor Class
*/
class Processor{
	Instruction IR;
	MemoryAddress PC;

	/*
		512 byte data memory
	*/
	DataElement DataMemory[MAX_DMEM_SIZE];

	/*
		512 instruction memory => 256 instructions
	*/
	Instruction InstrMemory[MAX_IMEM_SIZE];

	/*
		Register file of 16-bit regs
	*/
	Data RegisterFile[MAX_REGS];

public:

	/*Initialize data members*/
	SimulationProcessor(){
		IR.reset();
		PC.reset();

		memset(DataMemory, 0, sizeof(char) * MAX_DMEM_SIZE);
		for(int i=0; i<MAX_IMEM_SIZE; i++)
			InstrMemory[i].reset();
		
		for(int i=0; i<MAX_REGS; i++)
			RegisterFile[i] = 0;
	}

	/*Loads Instruction to InstrMemory*/
	int iload(const MemoryAddress &addr, const Instruction &i){
		// Maligned instruction address
		if(addr.address % 2)
			return -1;
		
		InstrMemory[addr.address >> 1] = i;  
		return 0;
	}

	/*Loads Data to InstrMemory*/
	void dload(const MemoryAddress &addr, const Data &val){
		DataMemory[addr.address] = val & 0xff;
		DataMemory[addr.address + 1] = (val >> 8) & 0xff;
	}

	void start(){
		do{
			/*Simulate Execution*/
		} while(PC.opcode != HLT);		
	}

	void generateDump(char *outfile){
		/* Save Memory Dump to outfile */
	}

} SimulationProcessor;

/*
	Compile Binary from Assembly 
	and Load to SimulationProcessor.
*/
void InitializeProcessor(char *infile){
	/* Compile Binary and load to Simulation Processor */
}

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("Usage: ./ProcessorSimulator.o <infile> <outfile>\n");
		return 1;
	}

	// Compile Assembly to binary and load
	InitializeProcessor(argv[1]);
	// Simulate Processor on binary
	SimulationProcessor.start();
	// Dump memory to file
	SimulationProcessor.generateDump(argv[2]);	

	return 0;
}