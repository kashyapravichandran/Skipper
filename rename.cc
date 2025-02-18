
#include "pipeline.h"


// Things we might have to add - Stall during difficult branch - Free List count 

////////////////////////////////////////////////////////////////////////////////////
// The Rename Stage has two sub-stages:
// rename1: Get the next rename bundle from the FQ.
// rename2: Rename the current rename bundle.
////////////////////////////////////////////////////////////////////////////////////

bool difficult_branch_flag = false;

void pipeline_t::rename1() {
   unsigned int i;

   ////////////////////////////////////////////////////////////////////////////////////
   // Try to get the next rename bundle.
   // Two conditions might prevent getting the next rename bundle, either:
   // (1) The current rename bundle is stalled in rename2.
   // (2) The FQ does not have enough instructions for a full rename bundle.
   ////////////////////////////////////////////////////////////////////////////////////

   // Check the first condition. Is the current rename bundle stalled, preventing
   // insertion of the next rename bundle? Check whether or not the pipeline register
   // between rename1 and rename2 still has a rename bundle.

   if (RENAME2[0].valid) {	// The current rename bundle is stalled.
      return;
   }

   // Check the second condition. Does the FQ have enough instructions for a full
   // rename bundle?

   if (!FQ.bundle_ready(dispatch_width)) {
      return;
   }

   // Get the next rename bundle:
   // The FQ has a rename bundle and there is space for it in the Rename Stage.
   for (i = 0; i < dispatch_width; i++) {
      RENAME2[i].valid = true;
      RENAME2[i].index = FQ.pop();
   }
}

void pipeline_t::rename2() {
   unsigned int i;
   unsigned int index;


   // Stall the rename2 sub-stage if either:
   // (1) There isn't a current rename bundle.
   // (2) The Dispatch Stage is stalled.
   // (3) There aren't enough rename resources for the current rename bundle.

   if (!RENAME2[0].valid ||	// First stall condition: There isn't a current rename bundle.
       DISPATCH[0].valid) {	// Second stall condition: The Dispatch Stage is stalled.
      return;
   }

   unsigned int num_chkpts = 0;
   unsigned int num_dest_regs = 0;
   // Third stall condition: There aren't enough rename resources for the current rename bundle.
   for (i = 0; i < dispatch_width; i++) {
      assert(RENAME2[i].valid);
      index = RENAME2[i].index;

      // FIX_ME #1
      // Count the number of instructions in the rename bundle that need a checkpoint (most branches).
      // Count the number of instructions in the rename bundle that have a destination register.
      // With these counts, you will be able to query the renamer for resource availability
      // (checkpoints and physical registers).
      //
      // Tips:
      // 1. The loop construct, for iterating through all instructions in the rename bundle (0 to dispatch_width),
      //    is already provided for you, above. Note that this comment is within the loop.
      // 2. At this point of the code, 'index' is the instruction's index into PAY.buf[] (payload).
      // 3. The instruction's payload has all the information you need to count resource needs.
      //    There is a flag in the instruction's payload that *directly* tells you if this instruction needs a checkpoint.
      //    Another field indicates whether or not the instruction has a destination register.
      if(PAY.buf[index].checkpoint && PAY.buf[index].pc!=SCIT->SCIT_get_PC()) num_chkpts++;
      if(PAY.buf[index].C_valid && PAY.buf[index].skipped_type!=2) num_dest_regs++; 
   }

   // FIX_ME #2


   // Check if the Rename2 Stage must stall due to any of the following conditions:
   // * Not enough free checkpoints.
   // * Not enough free physical registers.
   //
   // If there are not enough resources for the *whole* rename bundle, then stall the Rename2 Stage.
   // Stalling is achieved by returning from this function ('return').
   // If there are enough resources for the *whole* rename bundle, then do not stall the Rename2 Stage.
   // This is achieved by doing nothing and proceeding to the next statements.
   if(REN->stall_branch(num_chkpts)) return;
   if(REN->stall_reg(num_dest_regs)) return;
   
   // ---- Stalling Condition added for difficult Branches  
   // Condition when we are bringing in pmoves and skipped instruction stall condition reduces.
   
   
   for(i=0;i<dispatch_width;i++)
   {
		index = RENAME2[i].index;
		if(PAY.buf[index].pc == SCIT->SCIT_get_PC())
		{
   			if(REN->stall_skipper())
				return;
			if(REN->stall_reg(SCIT->SCIT_num_output() + num_dest_regs))
				return;
		}
	}
   
   
   // ---- 


   // Sufficient resources are available to rename the rename bundle.
   //
   for (i = 0; i < dispatch_width; i++) {
      assert(RENAME2[i].valid);
      index = RENAME2[i].index;

      // FIX_ME #3
      // Rename source registers (first) and destination register (second).
      //
      // Tips:
      // 1. At this point of the code, 'index' is the instruction's index into PAY.buf[] (payload).
      // 2. The instruction's payload has all the information you need to rename registers, if they exist. In particular:
      //    * whether or not the instruction has a first source register, and its logical register number
      //    * whether or not the instruction has a second source register, and its logical register number
      //    * whether or not the instruction has a third source register, and its logical register number
      //    * whether or not the instruction has a destination register, and its logical register number
      // 3. When you rename a logical register to a physical register, remember to *update* the instruction's payload with the physical register specifier,
      //    so that the physical register specifier can be used in subsequent pipeline stages.

      if(PAY.buf[index].skipped_type == 0) //normal instruction
	  {
		if(PAY.buf[index].A_valid) PAY.buf[index].A_phys_reg = REN->rename_rsrc(PAY.buf[index].A_log_reg);
      	if(PAY.buf[index].B_valid) PAY.buf[index].B_phys_reg = REN->rename_rsrc(PAY.buf[index].B_log_reg);
      	if(PAY.buf[index].D_valid) PAY.buf[index].D_phys_reg = REN->rename_rsrc(PAY.buf[index].D_log_reg);
      	if(PAY.buf[index].C_valid) PAY.buf[index].C_phys_reg = REN->rename_rdst(PAY.buf[index].C_log_reg);
      }
      else if(PAY.buf[index].skipped_type==1) //instruction in skipped block
      {
      	if(PAY.buf[index].A_valid) PAY.buf[index].A_phys_reg = REN->skipper_rename_src(PAY.buf[index].A_log_reg);
      	if(PAY.buf[index].B_valid) PAY.buf[index].B_phys_reg = REN->skipper_rename_src(PAY.buf[index].B_log_reg);
      	if(PAY.buf[index].D_valid) PAY.buf[index].D_phys_reg = REN->skipper_rename_src(PAY.buf[index].D_log_reg);
      	if(PAY.buf[index].C_valid) PAY.buf[index].C_phys_reg = REN->skipper_rename_dst(PAY.buf[index].C_log_reg);
	  }
	  else //pmove
	  {
	  	if(PAY.buf[index].A_valid) PAY.buf[index].A_phys_reg = REN->skipper_rename_src(PAY.buf[index].A_log_reg);
      	if(PAY.buf[index].B_valid) PAY.buf[index].B_phys_reg = REN->skipper_rename_src(PAY.buf[index].B_log_reg);
      	if(PAY.buf[index].D_valid) PAY.buf[index].D_phys_reg = REN->skipper_rename_src(PAY.buf[index].D_log_reg);
      	if(PAY.buf[index].C_valid) PAY.buf[index].C_phys_reg = REN->rename_skipped(PAY.buf[index].C_log_reg);
	  }
		
			


      // Get the instruction's branch mask.
      //
      // Tips:
      // 1. Every instruction gets a branch_mask. An instruction needs to know which branches it depends on, for possible squashing.
      // 2. The branch_mask is not held in the instruction's PAY.buf[] entry. Rather, it explicitly moves with the instruction
      //    from one pipeline stage to the next. Normally the branch_mask would be wires at this point in the logic but since we
      //    don't have wires place it temporarily in the RENAME2[] pipeline register alongside the instruction, until it advances
      //    to the DISPATCH[] pipeline register. The required left-hand side of the assignment statement is already provided for you below:
      //    RENAME2[i].branch_mask = ??;
      RENAME2[i].branch_mask = REN->get_branch_mask();


      // FIX_ME #5
      // If this instruction requires a checkpoint (most branches), then create a checkpoint.
      //
      // Tips:
      // 1. At this point of the code, 'index' is the instruction's index into PAY.buf[] (payload).
      // 2. There is a flag in the instruction's payload that *directly* tells you if this instruction needs a checkpoint.
      // 3. If you create a checkpoint, remember to *update* the instruction's payload with its branch ID
      //    so that the branch ID can be used in subsequent pipeline stages.

      if(PAY.buf[index].checkpoint && PAY.buf[index].pc!=SCIT->SCIT_get_PC()) PAY.buf[index].branch_ID = REN->checkpoint(); // PC check 
      //TODO: Maybe need to use another variable to keep track of whether a checkpoint has been skipped. After that dont skip any more checkpoint creations
	  
	  
	  // If there is a difficult branch
	  if(PAY.buf[index].pc==SCIT->SCIT_get_PC())
	  {
		// Create SIST entry here 
		uint64_t SIST_PC = SCIT->SCIT_get_PC();
		uint64_t SIST_recon = SCIT->SCIT_rpc();
		uint64_t output = SCIT->SCIT_num_output(), input = SCIT->SCIT_num_input();	  	
		uint64_t *outputreg = SCIT->SCIT_outputreg(), *inputreg=SCIT->SCIT_inputreg();
		uint64_t num_instr = SCIT->SCIT_get_num_instr();
		REN->create_SIST(SIST_PC,SIST_recon, input, output, inputreg, outputreg,num_instr);
			  	
	  }		

   }

   //
   // Transfer the rename bundle from the Rename Stage to the Dispatch Stage.
   //
   for (i = 0; i < dispatch_width; i++) {
      assert(RENAME2[i].valid && !DISPATCH[i].valid);
      RENAME2[i].valid = false;
      DISPATCH[i].valid = true;
      DISPATCH[i].index = RENAME2[i].index;
      DISPATCH[i].branch_mask = RENAME2[i].branch_mask;
   }
}
