#include<inttypes.h>
#include "SCIT_table.h"

SCIT_table::SCIT_table(uint64_t pc, uint64_t rpc, uint64_t insn, uint64_t* ipregs, uint64_t* opregs)
{
    PC = pc;
    RPC = rpc;
    num_insn = insn;

    input_regs = ipregs;
    output_regs = opregs;
}
