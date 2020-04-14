#include <inttypes.h>

class SCIT_table {

    uint64_t PC;
    uint64_t num_insn;
    uint64_t RPC;

    uint64_t* input_regs;
    uint64_t* output_regs;

  public:

    SCIT_table(uint64_t pc, uint64_t rpc, uint64_t insn, uint64_t* ipregs, uint64_t* opregs);

};
