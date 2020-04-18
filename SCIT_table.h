#include <inttypes.h>

#ifndef _SCIT_table_H_
#define _SCIT_table_h_

class SCIT_table {

    uint64_t PC;
    uint64_t num_insn;
    uint64_t RPC;
	
    uint64_t* input_regs;
    uint64_t* output_regs;
	uint64_t num_output, num_input;
	
  public:

    SCIT_table(uint64_t pc, uint64_t rpc, uint64_t insn, uint64_t* ipregs, uint64_t* opregs, uint64_t output, uint64_t input);
	uint64_t SCIT_get_PC();
	uint64_t SCIT_get_num_instr();
	uint64_t* SCIT_inputreg();
	uint64_t SCIT_num_input();
	uint64_t* SCIT_outputreg();
	uint64_t SCIT_num_output();
	uint64_t SCIT_rpc();
};
#endif 
