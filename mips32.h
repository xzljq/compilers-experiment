#ifndef _MIPS32_H
#define _MIPS32_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "senmatic.h"
#include "TreeNode.h"
#include "debug.h"
#include "ir.h"



struct RegDesc
{
    int use;//1-busy 0-idle
    Operand op;
};

struct VarDesc
{
    Operand op;
    int offset;//in frame
    struct VarDesc* next;
};



void init_asm(struct InterCodes* head);
void gen_mips(struct InterCodes* head);
void setBasicBlock(struct InterCodes* head);
struct InterCodes* process_block(struct InterCodes* head);
void RefreshRegList();
int Init_func(struct InterCodes* head);
int insert_VarDescList(Operand op,int cur_offset);
int mem_offset(Operand op);

int Ensure(Operand x,struct InterCodes* head);
void flush_reg(struct InterCodes* head);
OpList get_oplist(struct InterCodes* head);
int Allocate(Operand x,struct InterCodes* head);
char* Opname(Operand op);
int Opcmp(Operand op1,Operand op2);

#endif