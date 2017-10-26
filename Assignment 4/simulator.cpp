/*
	Program to simulate a processor

	Rahul Kejriwal
	CS14B023
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <bitset>
using namespace std;

/*
	Twos Complement Conversion
*/
#define TWOCOMP(x) (((x)>7)?x-16:x)
#define TWOCOMP2(x) (((x)>127)?x-256:x)

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
#define SD 10

#define JMP  12
#define BEQZ 13
#define HLT  14

/*
	Structure to hold instructions
*/
typedef struct Instruction{
	unsigned short opcode : 4;
	unsigned short op1 : 4;
	unsigned short op2 : 4;
	unsigned short op3 : 4;

	void reset(){
		opcode = 0;
		op1 = op2 = op3 = 0;
	}

	Instruction(){
		opcode = 0;
		op1 = op2 = op3 = 0;
	}

	Instruction(int op, int o1, int o2, int o3){
		opcode = op;
		op1 = o1;
		op2 = o2;
		op3 = o3;
	}

} Instruction;

/*
	Structure to hold 10-bit memory addresses
*/
typedef struct MemoryAddress{
	unsigned short isData : 1;
	unsigned short address : 9;

	void reset(){
		isData = 0;
		address = 0;
	}

	MemoryAddress(){
		isData  = 0;
		address = 0;
	}

	MemoryAddress(int iD, int add){
		isData = iD;
		address = add;
	} 

} MemoryAddress;

// Typedef-ed
typedef unsigned char  DataElement;
typedef signed short Data;

/*
	Simulation Processor Class
*/
class Processor{
	Instruction *IR;
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


	/*
		Private Functions: 
			Internal Implementations for Pipeline Stages Logic
	*/

	/* Stage 1: Instruction Fetch & Increment PC */
	void instrFetch(){
		IR = &InstrMemory[PC.address/2];
		PC.address += 2;
	}

	/* Stage 2: Decode & Operand Fetch */
	void decodeAndOperandFetch(Instruction *IR, unsigned short &opcode, unsigned short *regIds, Data *regVals, Data &ImmVal, unsigned short &Addr){
		opcode = IR->opcode;

		regIds[0] = IR->op1;
		regIds[1] = IR->op2;
		regIds[2] = IR->op3;
		
		regVals[0] = RegisterFile[reg1addr];
		regVals[1] = RegisterFile[reg2addr];
		regVals[2] = RegisterFile[reg3addr];
	
		ImmVal = TWOCOMP(IR->op3);
		Addr = TWOCOMP2((((unsigned short) (IR->op2)) << 4) | IR->op3);
	}

	/* Stage 3: Execute */
	void execute(unsigned short opcode, unsigned short *regIds, Data *regVals, Data ImmVal, unsigned short Addr, MemoryAddress currPC, Data &ALUResult, unsigned short &loc, MemoryAddress &nextPC){
		// Ac;tual Execution
		switch(opcode){
			case ADD: 	ALUResult = regVals[1] + regVals[2];
					  	break;
			
			case ADDI:	ALUResult = regVals[1] + ImmVal;
						break;
			
			case SUB:	ALUResult = regVals[1] - regVals[2];
					  	break;
			
			case SUBI:	ALUResult = regVals[1] - ImmVal;
					  	break;
			
			case MUL:	ALUResult = regVals[1] * regVals[2];
					  	break;
			
			case MULI:	ALUResult = regVals[1] * ImmVal;
					  	break;

			case LD:	loc = regVals[1] + regVals[2] - 512;
					  	break;

			case SD:	loc = regVals[0] + regVals[1] - 512;
						ALUResult = regVals[2];
					  	break;

			case JMP:	nextPC.address = currPC.address + Addr - 2;
						break;

			case BEQZ:	ALUResult = 0;
						if(regVals[0] == 0){
							nextPC.address = currPC.address + Addr - 2;
							ALUResult = 1;
						}
						break;
		
			case HLT: break; // do nothing, loop will terminate

			default: break; // silently skip instruction
		}
	}

	/* Stage 4: Memory Operations and branching */
	void memoryOpsAndBranch(Data ALUResult, unsigned short loc, MemoryAddress nextPC, unsigned short opcode, bool &flushFlag, Data &newALUResult){

		flushFlag = false;

		switch(opcode){
			case LD:	newALUResult = ((unsigned short) DataMemory[loc] << 8) | DataMemory[loc+1];
						break;

			case SD:	DataMemory[loc] =  ((unsigned short)ALUResult) >> 8 ;
						DataMemory[loc+1] = (ALUResult);
						break;

			case JMP:	PC.address = nextPC.address;
						// flush pipeline
						flushFlag = true;
						break;

			case BEQZ:	if(ALUResult){
							PC.address = nextPC.address;
							// flush pipeline
							flushFlag = true;
						}
						break;

			default:	newALUResult = ALUResult;
						break;
		}
	}

	/* Stage 5: Write vals to register file */
	void writeBack(Data ALUResult, unsigned short tgtReg, unsigned short opcode){
		if(	
			opcode == ADD  || 
			opcode == ADDI || 
			opcode == SUB  || 
			opcode == SUBI || 
			opcode == MUL  || 
			opcode == MULI || 
			opcode == LD 
		  ){
			RegisterFile[tgtReg] = ALUResult;	
		}
	}

public:

	/*
		Public Interface for the class
	*/

	/* Initialize data members */
	Processor(){
		IR = InstrMemory;
		PC.reset();

		memset(DataMemory, 0, sizeof(char) * MAX_DMEM_SIZE);
		for(int i=0; i<MAX_IMEM_SIZE; i++)
			InstrMemory[i].reset();
		
		for(int i=0; i<MAX_REGS; i++)
			RegisterFile[i] = 0;
	}

	/* Loads Instruction to InstrMemory */
	int iload(const MemoryAddress &addr, const Instruction &i){
		// Maligned instruction address
		if(addr.address % 2)
			return -1;
		
		InstrMemory[addr.address >> 1] = i;  
		return 0;
	}

	/* Loads Data to InstrMemory */
	void dload(const MemoryAddress &addr, const Data &val){
		DataMemory[addr.address] = (val >> 8) & 0xff;
		DataMemory[addr.address + 1] = val & 0xff;
	}

	/* Loads initial values of registers */
	void reg_init(const DataElement regAddr, const Data val){
		if(regAddr < 0 || regAddr > 15)		return;
		RegisterFile[regAddr] = val;
	}

	/* Simulation Begin */
	void start(){
		
		// Flags to suppress pipeline stage work in a cycle
		#define WB_STAGE  4
		#define MEM_STAGE 3
		#define EXE_STAGE 2
		#define DEC_STAGE 1
		#define FI_STAGE  0
		int locks[5] = { 0, 1, 2, 3, 4 }

		// Simulate in reverse order of stages to avoid temporary variables (buffer registers)
		do{
			/* Interface Variable Declarations */
			
			MemoryAddress oldPC;
			unsigned short opcode;
			unsigned short regIds[3];
			Data regVals[3];
			Data ImmVal;
			unsigned short Addr;

			Data ALUResult;
			unsigned short loc;
			MemoryAddress nextPC; nextPC.isData = 0;
			unsigned short oldOpcode;

			Data newALUResult;
			unsigned short tgtReg;
			unsigned short oldTgtReg;
			unsigned short olderOpcode;

			bool flushFlag;

			/*
				Stage 5 - Write Back
			*/

			if(!locks[WB_STAGE]){
				// write back
				writeBack(newALUResult, oldTgtReg, olderOpcode);
			
				// Check termination
				if(olderOpcode == HLT)
					break;
			}
			else
				locks[WB_STAGE]--;

			/*
				Stage 4 - Memory Operations & PC updation & flushing
			*/

			olderOpcode = oldOpcode;
			oldTgtReg = tgtReg;

			if(!locks[MEM_STAGE]){
				/*
					Notice no data forwarding is reqd for data stored in memory cause memory read 
					and writes are not separated by any pipeline stages.
				*/

				// Mem Ops & branch
				memoryOpsAndBranch(ALUResult, loc, nextPC, oldOpcode, flushFlag, newALUResult);
			}
			else
				locks[MEM_STAGE]--;

			/*
				Stage 3 - Execute 
			*/

			oldOpcode = opcode;
			tgtReg = regIds[0];

			if(!locks[EXE_STAGE]){
				// Data Forwarding here


				// Execute
				execute(opcode, regIds, regVals, ImmVal, Addr, oldPC, ALUResult, loc, nextPC);				
			}
			else
				locks[EXE_STAGE]--;

			/*
				Stage 2 - Decode and Fetch Operands 
			*/

			oldPC = PC;

			if(!locks[DEC_STAGE]){
				decodeAndOperandFetch(IR, opcode, regIds, regVals, ImmVal, Addr);
			}
			else
				locks[DEC_STAGE]--;

			/*
				Stage 1 - Fetch & Increment PC
			*/
			
			if(!locks[FI_STAGE]){
				instrFetch();
			}
			else
				locks[FI_STAGE]--;

			/*
				Flush pipeline if triggered
			*/
			if(flushFlag){
				locks[0] = 0; locks[1] = 1; locks[2] = 2; locks[3] = 3; locks[4] = 4;
			}

		} while(true);
	}

	/* Save Memory Dump to outfile */
	void generateDump(char *outfile){
		fstream fil(outfile, ios::out);

		for(int i=0; i<MAX_IMEM_SIZE; i++){
			fil << ((i<5)?"000":((i<50)?"00":"0"))  << (i << 1) << " : " 
				<< bitset<4>(InstrMemory[i].opcode) << " "
				<< bitset<4>(InstrMemory[i].op1) << endl;
			fil << ((i<5)?"000":((i<50)?"00":"0"))  << (i << 1) + 1 << " : "
				<< bitset<4>(InstrMemory[i].op2) << " "
				<< bitset<4>(InstrMemory[i].op3) << endl;
		}
	
		for(int i=0; i<MAX_DMEM_SIZE; i+=1){
			fil << ((512+i < 1000)?"0":"") << 512+i << " : "
				<< bitset<4>(DataMemory[i] >> 4) << " " 
				<< bitset<4>(DataMemory[i]) << endl;
		}
	}

} SimulationProcessor;

/*
	Compile Binary from Assembly 
	and Load to SimulationProcessor.
*/
void InitializeProcessor(char *infile){
	/* Compile Binary and load to Simulation Processor */
	FILE *fp = fopen(infile, "r");
	int val1, val2;
	
	// Register Initialization
	for(int i=0; i<16; i++){
		fscanf(fp, "%d", &val1);
		SimulationProcessor.reg_init(i, val1);
	}

	// Memory Initialization
	fscanf(fp, "%d %d", &val1, &val2);
	while(val1 != -1){
		SimulationProcessor.dload(MemoryAddress(1, val1-512), val2);
		fscanf(fp, "%d %d", &val1, &val2);
	}

	// Instructions Load
	int opcode, op1, op2, op3;
	int address = 0;

	fscanf(fp, "%d %d %d %d", &opcode, &op1, &op2, &op3);
	while(opcode != -1){
		SimulationProcessor.iload(MemoryAddress(0, address), Instruction(opcode, op1, op2, op3));
		address += 2;
		fscanf(fp, "%d %d %d %d", &opcode, &op1, &op2, &op3);
	}
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