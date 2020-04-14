#include<inttypes.h>
#include "SCIT_table.h"

SCIT_table::SCIT_table(uint64_t pc, uint64_t rpc, uint64_t insn, uint64_t* ipregs, uint64_t* opregs, uint64_t output, uint64_t input)
{
    PC = pc;
    RPC = rpc;
    num_insn = insn;

    input_regs = ipregs;
    output_regs = opregs;
    
    num_input = input;
	num_output = output; 
    
}

uint64_t SCIT_table::SCIT_get_PC()
{
	return PC;
	
}

uint64_t SCIT_table::SCIT_get_num_instr()
{
	return num_insn;
}

uint64_t* SCIT_table::SCIT_inputreg()
{
	return input_regs;
	
}

uint64_t* SCIT_table::SCIT_outputreg()
{
	return output_regs;
}

uint64_t SCIT_table::SCIT_num_input()
{
	return num_input;
}

uint64_t SCIT_table::SCIT_num_output()
{
	return num_output;
}
