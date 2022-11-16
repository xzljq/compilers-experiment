#ifndef _SEMANTIC_H

#define	_SEMANTIC_H


#include "syntax.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TreeNode.h"
#include "debug.h"
#define TABLESIZE 0x3fff
#define STACKSIZE 0x1000

#define INTT 0
#define FLOATT 1

#define DEFINED 1
#define DECLARED 0

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
//typedef struct Symbol Symbol;
struct Type_
{
	enum { BASIC, ARRAY, STRUCTURE, FUNCTION , STRUCTNAME} kind;
	union
	{
		// 基本类型
		int basic; //0-int 1-float
		// 数组类型信息包括元素类型与数组大小构成
		struct { Type elem; int size; } array;
		// 结构体类型信息是一个链表
        FieldList structure;
		// 对于函数至少要记录其返回类型、参数个数以及参数类型
		struct { Type ret_type; int num; FieldList para_list; int defined;/*0-only declared 1-have defined*/int line;} function;

		//name of struct difinition
		Type structname_type;
	} u;
};

int FL_equal(FieldList a,FieldList b);//1-true-equal 0-unequal
int Type_equal(Type a,Type b);//1-true-equal 0-unequal

struct FieldList_
{
	char* name; // 域的名字
	Type type; // 域的类型
	FieldList tail; // 下一个域
};

unsigned int hash_pjw(char* name);

struct Symbol
{
	char* name;
	Type type;
	struct Symbol* hash_next;
	struct Symbol* stack_next;
	int stack_depth;
};

struct SymbolTable
{
	struct Symbol* HashTable[TABLESIZE];
	struct Symbol* Stack[STACKSIZE];
	int curdepth;
};

// struct StructTable
// {
	
// }

struct Symbol* SymbolFind(struct SymbolTable* ST,char* name);
void SymbolInsert(struct SymbolTable* ST,struct Symbol* s);
void DeleteCurdepth(struct SymbolTable* ST);
void PrintST(struct SymbolTable* ST);
void Insert_func(char* str);

void senmatic_check(struct TreeNode* root);
void program(struct TreeNode* root);
void extdeflist(struct TreeNode* root);
void extdef(struct TreeNode* root);
Type specifier(struct TreeNode* root);
Type structspecifier(struct TreeNode* root);
FieldList deflist_struct(struct TreeNode* root);
FieldList def_struct(struct TreeNode* root);
FieldList declist_struct(Type t,struct TreeNode* root);
void dec_struct(FieldList FL,Type t,struct TreeNode* root);
//void vardec_struct(FieldList FL,Type t,struct TreeNode* root);
void extdeclist(Type t,struct TreeNode* root);
struct Symbol* vardec(Type t,struct TreeNode* root);
struct Symbol* fundec(int define,Type ret_t,struct TreeNode* root);
void varlist(Type t,struct TreeNode* root);
void compst(struct Symbol* S,struct TreeNode* root);

FieldList deflist_func(struct TreeNode* root);
void def_func(struct TreeNode* root);
void declist_func(Type t,struct TreeNode* root);
void dec_func(Type t,struct TreeNode* root);
void stmtlist(struct Symbol* S, struct TreeNode* root);
void stmt(struct Symbol* S, struct TreeNode* root);
Type expp(struct TreeNode* root);

FieldList check_struct(FieldList t,char* name);
int check_funpara(FieldList FL,struct TreeNode* root);//1-ok 0-not ok
int check_func_equal(Type t1,Type t2);
void check_only_declared_func(struct SymbolTable* ST);
#endif