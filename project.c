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
    /* Step 1: advance PC past the current instruction */
    *PC = *PC + 4;
 
    /* Step 2: branch — PC-relative, word-addressed offset already sign-extended.
     * The offset from sign_extend represents words; shift left 2 to get bytes.
     * Only taken when Branch control signal is set AND the ALU Zero flag is 1. */
    if (Branch && Zero)
        *PC = *PC + (extended_value << 2);
 
    /* Step 3: jump — 26-bit word address (jsec) shifted left 2 gives a
     * 28-bit byte address.  The upper 4 bits come from the updated PC. */
    if (Jump)
        *PC = (*PC & 0xF0000000) | (jsec << 2);
}
 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SPIMCORE

#define MEM(addr) (Mem[addr >> 2])

typedef struct
{
	char RegDst;
	char Jump;
	char Branch;
	char MemRead;
	char MemtoReg;
	char ALUOp;
	char MemWrite;
	char ALUSrc;
	char RegWrite;
}struct_controls;

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero);

/* fetch instruction from memory */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction);

/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec);

/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls);

/* read_register */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2);

/* sign_extend */
void sign_extend(unsigned offset,unsigned *extended_value);

/* ALU */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero);

/* read/write memory */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem);

/* write to register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg);

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC);

#define SPIMCORE
#endif
30bf0769cd90ba1c4d65507d11c61e7da395417c
