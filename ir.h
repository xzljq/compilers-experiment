#ifndef _IR_H

#define	_IR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "senmatic.h"
#include "TreeNode.h"
#include "debug.h"

typedef struct Operand_* Operand;
struct Operand_ {
    enum { VARIABLE, CONSTANT, ADDRESS, LABLE, REL, FUNC} kind;
    union {
        char* var_name;
        int value;
        char* lable_name;
        char* relop;
        char* func_name;
    } u;
};

struct InterCode
{
    enum { IC_ASSIGN, IC_ADD, IC_SUB, IC_MUL, IC_DIV, IC_LABLE, IC_FUNC,
            IC_RETURN, IC_GOTO, IC_IFGOTO, IC_READ, IC_WRITE, IC_CALL, IC_ARG, 
            IC_DEC, IC_PARAM, IC_ADDR, IC_R_DEREF, IC_L_DEREF} kind;
    union {
        struct { Operand right, left; } assign;
        struct { Operand result, op1, op2; } binop;
        struct { Operand op_lable;} lable;
        struct { Operand op_func; } func;
        struct { Operand op_return; } ret;
        struct { Operand op_goto;} gt;
        struct { Operand x,relop,y,z; } ifgt;
        struct { Operand op_read; } read;
        struct { Operand op_write; } write;
        struct { Operand x, f; } call;
        struct { Operand x; } arg;
        struct { Operand x; int size; } dec;
        struct { Operand x; } param;
        struct { Operand right, left; } addr;
        struct { Operand right, left; } r_deref;
        struct { Operand right, left; } l_deref;
    } u;
};

struct InterCodes 
{ 
    struct InterCode code;
    struct InterCodes *prev, *next;
};

struct operandlist
{
    Operand op;
    struct operandlist *prev,*next;
};
void writeToFile(struct InterCodes* head,FILE* f);


Operand new_temp();
Operand new_lable();
Operand get_relop();
struct InterCodes* new_code();
Operand new_constant(int value);
int szof(Type t);

void connect_ic(struct InterCodes* code1,struct InterCodes* code2);
void write_OP(Operand op,FILE* f);
void print_OP(Operand op);


struct InterCodes* translate_Exp(struct TreeNode* Exp, struct SymbolTable* sym_table, Operand place);
struct InterCodes* translate_Cond(struct TreeNode* Exp,Operand label_true, Operand label_false,struct SymbolTable* sym_table);
struct InterCodes* translate_Stmt(struct TreeNode* Stmt,struct SymbolTable* sym_table);
struct InterCodes* translate_CompSt(struct TreeNode* CompSt,struct SymbolTable* sym_table);
struct InterCodes* translate_Args(struct TreeNode* Args,struct SymbolTable* sym_table,struct operandlist** arg_list);
struct InterCodes* translate_StmtList(struct TreeNode* StmtList,struct SymbolTable* sym_table);
void generate_ic(struct TreeNode* root);
struct InterCodes* translate_program(struct TreeNode* Program);
struct InterCodes* translate_ExtDefList(struct TreeNode* ExtDefList);
struct InterCodes* translate_ExtDef(struct TreeNode* ExtDef);
struct InterCodes* translate_FunDec(struct TreeNode* FunDec);
struct InterCodes* translate_VarList(struct TreeNode* VarList);
struct InterCodes* translate_ParamDec(struct TreeNode* ParamDec);
struct InterCodes* translate_DefList(struct TreeNode* DefList);
struct InterCodes* translate_Def(struct TreeNode* Def);
struct InterCodes* translate_DecList(struct TreeNode* DecList);
struct InterCodes* translate_Dec(struct TreeNode* Dec);
struct InterCodes* translate_VarDec(struct TreeNode* VarDec,Operand place);
#endif