#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	if (ALUControl == 0)
	{
		*ALUresult = A + B;
	}
	else if (ALUControl == 1)
	{
		*ALUresult = A - B;
	}
	else if (ALUControl == 2)
	{
		*ALUresult = A & B;
	}
	else if (ALUControl == 3)
	{
		*ALUresult = A | B;
	}
	else if (ALUControl == 4)
	{
		if ((int) A < (int) B)
			*ALUresult = 1;
		else
			*ALUresult = 0;
	}
	else if (ALUControl == 5)
	{
		if (A < B)
			*ALUresult = 1;
		else
			*ALUresult = 0;
	}
	else if (ALUControl == 6)
	{
		*ALUresult = B << 16;
	}

	if (*ALUresult == 0)
	{
		*Zero = 1;
	}
	else
	{
		*Zero = 0;
	}
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	if (PC % 4 != 0)
		return 1;

	*instruction = Mem[PC / 4];
	return 0;
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
	else if (op == 0xc)
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
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	if (offset & 0x8000)
		*extended_value = offset | 0xFFFF0000;
	else
		*extended_value = offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	unsigned input2;
	char ALUControl;

	/* I-type instructions use the extended immediate instead of data2. */
	if (ALUSrc == 1)
	{
		input2 = extended_value;
	}
	else
	{
		input2 = data2;
	}

	if (ALUOp == 0)
	{
		ALUControl = 0;
	}
	else if (ALUOp == 1)
	{
		ALUControl = 1;
	}
	else if (ALUOp == 2)
	{
		ALUControl = 4;
	}
	else if (ALUOp == 3)
	{
		ALUControl = 5;
	}
	else if (ALUOp == 4)
	{
		ALUControl = 2;
	}
	else if (ALUOp == 5)
	{
		ALUControl = 3;
	}
	else if (ALUOp == 6)
	{
		ALUControl = 6;
	}
	/* R-type uses the funct field to decide the ALU action. */
	else if (ALUOp == 7)
	{
		if (funct == 0x20)
			ALUControl = 0;
		else if (funct == 0x22)
			ALUControl = 1;
		else if (funct == 0x24)
			ALUControl = 2;
		else if (funct == 0x25)
			ALUControl = 3;
		else if (funct == 0x2A)
			ALUControl = 4;
		else if (funct == 0x2B)
			ALUControl = 5;
		else
			return 1;
	}
	else
	{
		return 1;
	}

	ALU(data1,input2,ALUControl,ALUresult,Zero);
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
	*PC = *PC + 4;

	if (Branch && Zero)
		*PC = *PC + (extended_value << 2);

	if (Jump)
		*PC = (*PC & 0xF0000000) | (jsec << 2);
}
