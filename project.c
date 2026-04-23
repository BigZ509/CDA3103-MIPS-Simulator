 HEAD
#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

	}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	
if (PC % 4 != 0)
{
    return 1;
	}
else
{
    *instruction = Mem[PC >> 2];
    return 0;
	}
	
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	
*op = (instruction >> 26) & 0x3F;
*r1 = (instruction >> 21) & 0x1F;
*r2 = (instruction >> 16) & 0x1F;
*r3 = (instruction >> 11) & 0x1F;
*funct = instruction & 0x3F;
*offset = instruction & 0xFFFF;
*jsec = instruction & 0x03FFFFFF;
	
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

	}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	
*data1 = Reg[r1];
*data2 = Reg[r2];	
	
	}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	
if ((offset & 0x8000) != 0)
{
    *extended_value = offset | 0xFFFF0000;
	}
else
{
    *extended_value = offset;
	}
	
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    unsigned B = ALUSrc ? extended_value : data2;
    char ALUControl;

    if      (ALUOp == 0) ALUControl = 0;        /* add */
    else if (ALUOp == 1) ALUControl = 1;        /* subtract */
    else if (ALUOp == 2)                        /* R-type */
    {
        if      (funct == 0x20) ALUControl = 0; /* add  */
        else if (funct == 0x22) ALUControl = 1; /* sub  */
        else if (funct == 0x24) ALUControl = 2; /* AND  */
        else if (funct == 0x25) ALUControl = 3; /* OR   */
        else if (funct == 0x2A) ALUControl = 4; /* slt  */
        else if (funct == 0x2B) ALUControl = 5; /* sltu */
        else return 1;
    }
    else if (ALUOp == 3) ALUControl = 2;        /* andi */
    else if (ALUOp == 4) ALUControl = 3;        /* ori  */
    else if (ALUOp == 5) ALUControl = 6;        /* lui  */
    else if (ALUOp == 6) ALUControl = 4;        /* slti */
    else if (ALUOp == 7) ALUControl = 5;        /* sltiu */
    else return 1;

    ALU(data1, B, ALUControl, ALUresult, Zero);
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC = *PC + 4;                                          /* advance PC */

    if (Branch && Zero)                                     /* branch */
        *PC = *PC + (extended_value << 2);

    if (Jump)                                               /* jump */
        *PC = (*PC & 0xF0000000) | (jsec << 2);
}

#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	/* R-type: use registers and write result to rd. */
	if (op == 0x0) 
	{
		controls->RegDst = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 7;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 1;
		return 0;
	}
	/* j: jump to a new instruction address. */
	else if (op == 0x2) 
	{
		controls->RegDst = 2;
		controls->Jump = 1;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 2;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 2;
		controls->RegWrite = 0;
		return 0;
	}
	/* beq: compare two registers and branch if equal. */
	else if (op == 0x4) 
	{
		controls->RegDst = 2;
		controls->Jump = 0;
		controls->Branch = 1;
		controls->MemRead = 0;
		controls->MemtoReg = 2;
		controls->ALUOp = 1;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 0;
		return 0;
	}
	/* addi: add an immediate value and store the result in rt. */
	else if (op == 0x8) 
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}
	/* slti: set rt to 1 if rs is less than the immediate value. */
	else if (op == 0xa) 
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 2;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}
	/* sltiu: unsigned version of set less than immediate. */
	else if (op == 0xb) 
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 3;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}
	/* andi: AND a register value with the immediate. */
	else if (op == 0xc) /* andi */
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 4;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}
	/* ori: OR a register value with the immediate. */
	else if (op == 0xd) 
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 5;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}
	/* lui: load the immediate value into the upper half of the register. */
	else if (op == 0xf) 
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 6;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}
	/* lw: load word from memory into a register. */
	else if (op == 0x23) 
	{
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 1;
		controls->MemtoReg = 1;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		return 0;
	}
	/* sw: store word from a register into memory. */
	else if (op == 0x2b) 
	{
		controls->RegDst = 2;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 2;
		controls->ALUOp = 0;
		controls->MemWrite = 1;
		controls->ALUSrc = 1;
		controls->RegWrite = 0;
		return 0;
	}
	return 1;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    /* Pick the second ALU operand: register data2 or sign-extended immediate */
    unsigned B = ALUSrc ? extended_value : data2;

    /*
     * ALUOp encoding (set by instruction_decode):
     *   0  -> add          (lw / sw / addi)
     *   1  -> subtract     (beq: A - B, check Zero)
     *   2  -> R-type       (use funct field to determine operation)
     *   3  -> AND immediate
     *   4  -> OR  immediate
     *   5  -> lui          (shift left 16)
     *   6  -> set-less-than immediate (slti)
     *   7  -> set-less-than immediate unsigned (sltiu)
     *
     * funct encoding for R-type (ALUOp == 2):
     *   0x20 (32) -> add
     *   0x22 (34) -> subtract
     *   0x24 (36) -> AND
     *   0x25 (37) -> OR
     *   0x2A (42) -> set-less-than (signed)
     *   0x2B (43) -> set-less-than unsigned
     */

    char ALUControl;

    switch (ALUOp)
    {
        case 0: /* add */
            ALUControl = 0;
            break;

        case 1: /* subtract (beq) */
            ALUControl = 1;
            break;

        case 2: /* R-type: decode funct */
            switch (funct)
            {
                case 0x20: ALUControl = 0; break; /* add  */
                case 0x22: ALUControl = 1; break; /* sub  */
                case 0x24: ALUControl = 2; break; /* AND  */
                case 0x25: ALUControl = 3; break; /* OR   */
                case 0x2A: ALUControl = 4; break; /* slt  (signed)   */
                case 0x2B: ALUControl = 5; break; /* sltu (unsigned) */
                default:
                    return 1; /* halt: unrecognised funct */
            }
            break;

        case 3: /* andi */
            ALUControl = 2;
            break;

        case 4: /* ori */
            ALUControl = 3;
            break;

        case 5: /* lui */
            ALUControl = 6;
            break;

        case 6: /* slti (signed) */
            ALUControl = 4;
            break;

        case 7: /* sltiu (unsigned) */
            ALUControl = 5;
            break;

        default:
            return 1; /* halt: unrecognised ALUOp */
    }

    ALU(data1, B, ALUControl, ALUresult, Zero);
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if ((MemRead || MemWrite) && (ALUresult % 4 != 0))
		return 1;

	if (MemRead)
		*memdata = Mem[ALUresult / 4];

	if (MemWrite)
		Mem[ALUresult / 4] = data2;

	return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	unsigned dest;
	unsigned value;

	if (!RegWrite)
		return;

	if (RegDst == 1)
		dest = r3;
	else
		dest = r2;

	if (MemtoReg == 1)
		value = memdata;
	else
		value = ALUresult;

	if (dest != 0)
		Reg[dest] = value;
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    /* Step 1: advance PC past the current instruction */
    *PC = *PC + 4;

    /* Step 2: branch - PC-relative, word-addressed offset already sign-extended.
     * The offset from sign_extend represents words; shift left 2 to get bytes.
     * Only taken when Branch control signal is set AND the ALU Zero flag is 1. */
    if (Branch && Zero)
        *PC = *PC + (extended_value << 2);

    /* Step 3: jump - 26-bit word address (jsec) shifted left 2 gives a
     * 28-bit byte address. The upper 4 bits come from the updated PC. */
    if (Jump)
        *PC = (*PC & 0xF0000000) | (jsec << 2);
}
 205e29b9a73560637981b974976f07f434ed7155
