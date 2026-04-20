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