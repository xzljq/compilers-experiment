%{
	//#define YYDEBUG 1
	#define YYSTYPE struct TreeNode*
    #include <stdio.h>
	#include <stdarg.h>
	#include "lex.yy.c"
	int error=0;
	struct TreeNode* root;
	struct TreeNode* createfather(char *name, int line,int num,...);
	void print(struct TreeNode* node,int depth);
%}

//tokens
%token  INT
%token  FLOAT
%token  ID
%token  SEMI
%token  COMMA
%token  ASSIGNOP
%token  RELOP
%token  PLUS MINUS STAR DIV
%token  AND
%token  OR
%token  DOT
%token  NOT
%token  TYPE
%token  LP
%token  RP
%token  LB
%token  RB
%token  LC
%token  RC
%token  STRUCT
%token  RETURN
%token  IF
%token  ELSE
%token  WHILE

// C−−语言中运算符的优先级和结合性
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT
%left LB RB
%left LP RP
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
//7.1.2 High-level Definitions
Program: ExtDefList	{$$=createfather("Program",@1.first_line,1,$1);root=$$;/*if(error==0) print(root,0);*/}
	;
ExtDefList: ExtDef ExtDefList {$$=createfather("ExtDefList",@1.first_line,2,$1,$2);}				
    | {$$=createfather("ExtDefList",0,0);}
	;
ExtDef : Specifier ExtDecList SEMI {$$=createfather("ExtDef",@1.first_line,3,$1,$2,$3);}
	| Specifier SEMI {$$=createfather("ExtDef",@1.first_line,2,$1,$2);}
	| Specifier FunDec CompSt {$$=createfather("ExtDef",@1.first_line,3,$1,$2,$3);}
	//| Specifier ExtDecList error SEMI {error+=1;}
	;
ExtDecList : VarDec {$$=createfather("ExtDecList",@1.first_line,1,$1);}
	| VarDec COMMA ExtDecList {$$=createfather("ExtDecList",@1.first_line,3,$1,$2,$3);}
	;
//7.1.3 Specifiers
Specifier : TYPE {$$=createfather("Specifier",@1.first_line,1,$1);}
	| StructSpecifier {$$=createfather("Specifier",@1.first_line,1,$1);}
	;
StructSpecifier : STRUCT OptTag LC DefList RC {$$=createfather("StructSpecifier",@1.first_line,5,$1,$2,$3,$4,$5);}
	| STRUCT Tag {$$=createfather("StructSpecifier",@1.first_line,2,$1,$2);}
	//| STRUCT OptTag LC DefList error RC {error+=1;}
	;
OptTag : ID {$$=createfather("OptTag",@1.first_line,1,$1);}
	| {$$=createfather("OptTag",0,0);}
	;
Tag : ID {$$=createfather("Tag",@1.first_line,1,$1);}
	;
//7.1.4 Declarators
VarDec : ID {$$=createfather("VarDec",@1.first_line,1,$1);}
	| VarDec LB INT RB {$$=createfather("VarDec",@1.first_line,4,$1,$2,$3,$4);}
	//| VarDec LB error RB {error+=1;}
	;
FunDec : ID LP VarList RP {$$=createfather("FunDec",@1.first_line,4,$1,$2,$3,$4);}
	| ID LP RP {$$=createfather("FunDec",@1.first_line,3,$1,$2,$3);}
	//| ID LP error RP {error+=1;}
	;
VarList : ParamDec COMMA VarList {$$=createfather("VarList",@1.first_line,3,$1,$2,$3);}
	| ParamDec {$$=createfather("VarList",@1.first_line,1,$1);}
	;
ParamDec : Specifier VarDec {$$=createfather("ParamDec",@1.first_line,2,$1,$2);}
	;
//7.1.5 Statements
CompSt : LC DefList StmtList RC {$$=createfather("CompSt",@1.first_line,4,$1,$2,$3,$4);}
	//| error RC {error+=1;}
	;
StmtList : Stmt StmtList {$$=createfather("StmtList",@1.first_line,2,$1,$2);}
	| {$$=createfather("StmtList",0,0);}
	;
Stmt : Exp SEMI {$$=createfather("Stmt",@1.first_line,2,$1,$2);}
	| CompSt {$$=createfather("Stmt",@1.first_line,1,$1);}
	| RETURN Exp SEMI {$$=createfather("Stmt",@1.first_line,3,$1,$2,$3);}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$=createfather("Stmt",@1.first_line,5,$1,$2,$3,$4,$5);}
	| IF LP Exp RP Stmt ELSE Stmt {$$=createfather("Stmt",@1.first_line,7,$1,$2,$3,$4,$5,$6,$7);}
	| WHILE LP Exp RP Stmt {$$=createfather("Stmt",@1.first_line,5,$1,$2,$3,$4,$5);}
	| error SEMI {error+=1;}
	| IF LP Exp error RP Stmt ELSE Stmt
	;
//7.1.6 Local Definitions
DefList : Def DefList {$$=createfather("DefList",@1.first_line,2,$1,$2);}
	| {$$=createfather("DefList",0,0);}
	; 	
Def : Specifier DecList SEMI {$$=createfather("Def",@1.first_line,3,$1,$2,$3);}
	| Specifier DecList error SEMI {error+=1;}
	;
DecList : Dec {$$=createfather("DecList",@1.first_line,1,$1);}
	| Dec COMMA DecList {$$=createfather("DecList",@1.first_line,3,$1,$2,$3);}
	;
Dec : VarDec {$$=createfather("Dec",@1.first_line,1,$1);}
	| VarDec ASSIGNOP Exp {$$=createfather("Dec",@1.first_line,3,$1,$2,$3);}
	;
//7.1.7 Expressions
Exp : Exp ASSIGNOP Exp {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}
	| Exp AND Exp {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}
	| Exp OR Exp {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}
	| Exp RELOP Exp {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}
	| Exp PLUS Exp {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}	
	| Exp MINUS Exp {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}
	| Exp STAR Exp {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}
	| Exp DIV Exp {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}	
	| LP Exp RP {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}	
	| MINUS Exp {$$=createfather("Exp",@1.first_line,2,$1,$2);}
	| NOT Exp {$$=createfather("Exp",@1.first_line,2,$1,$2);}
	| ID LP Args RP {$$=createfather("Exp",@1.first_line,4,$1,$2,$3,$4);}
	| ID LP RP {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}
	| Exp LB Exp RB	{$$=createfather("Exp",@1.first_line,4,$1,$2,$3,$4);}
	| Exp DOT ID {$$=createfather("Exp",@1.first_line,3,$1,$2,$3);}
	| ID {$$=createfather("Exp",@1.first_line,1,$1);}
	| INT {$$=createfather("Exp",@1.first_line,1,$1);}
	| FLOAT {$$=createfather("Exp",@1.first_line,1,$1);}
	//| LB Exp error RP {error+=1;}
	;
Args : Exp COMMA Args {$$=createfather("Args",@1.first_line,3,$1,$2,$3);}
	| Exp {$$=createfather("Args",@1.first_line,1,$1);}
	;
%%

struct TreeNode* createfather(char *name, int line,int num,...)
{
	struct TreeNode* cur = (struct TreeNode*)malloc(sizeof(struct TreeNode));
	cur->line=line;
	cur->istoken=0;
	strcpy(cur->name,name);
	cur->childnum=num;
	
	va_list args;
	va_start(args, num);//获取可变参数列表的第一个参数的地址

	for(int i = 0;i<num;i++)
	{
		cur->childlist[i]=va_arg(args, struct TreeNode*);
	}
	return cur;
}

yyerror(char* msg) {
    fprintf(stderr, "Error type B at Line %d:%s\n",yylineno,msg);
}

void print(struct TreeNode* node,int depth)
{
	if(node->istoken==0)
	{
		if(node->childnum==0)//语法单元并且该语法单元产生了
		{
			return;
		}
		else//语法单元并且该语法单元没有产生
		{
			for(int i=0;i<depth;i++)
			{
				printf("  ");
			}
			printf("%s",node->name);
			printf(" (%d)\n",node->line);
		}
	}
	else//词法单元
	{
		for(int i=0;i<depth;i++)
		{
			printf("  ");
		}
		printf("%s",node->name);
		if(strcmp(node->name,"ID")==0) printf(": %s",node->extra);
		if(strcmp(node->name,"TYPE")==0) printf(": %s",node->extra);
		if(strcmp(node->name,"INT")==0) printf(": %d",node->INT);
		if(strcmp(node->name,"FLOAT")==0) printf(": %f",node->FLOAT);
		printf("\n");
	}
	for(int i=0;i<node->childnum;i++)
	{
		print(node->childlist[i],depth+1);
	}
}