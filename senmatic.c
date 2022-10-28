#include "senmatic.h"

extern struct TreeNode* root;

static struct SymbolTable ST;

unsigned int hash_pjw(char* name)
{ 
	unsigned int val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
	} 
	return val;
}

int FL_equal(FieldList a,FieldList b)//1-true-equal 0-unequal
{
    if(a==NULL&&b==NULL) return 1;
    if(a==NULL||b==NULL) return 0;

    if(Type_equal(a->type,b->type)==1)
        return FL_equal(a->tail,b->tail);
    else
        return 0;
}

int Type_equal(Type a,Type b)//1-true-equal 0-unequal
{
    if(a==NULL||b==NULL)
        return 0;
    if(a->kind==b->kind)
    {
        if(a->kind==BASIC)
        {
            if(a->u.basic==b->u.basic) return 1;
            else return 0;
        }
        else if(a->kind==ARRAY)
        {
            return Type_equal(a->u.array.elem,b->u.array.elem);
        }
        else if(a->kind==STRUCTURE)
        {
            if(Type_equal(a->u.structure->type,b->u.structure->type)==1)
            {
                return FL_equal(a->u.structure,b->u.structure);
            }
            else
            {
                return 0;
            }
        }
        else
        {
            printf("error:function in the left of =\n");
            return 0;
        }
    }
    else
    {
        if(b->kind==FUNCTION)
        {
            return Type_equal(a,b->u.function.ret_type);
        }
        else return 0;
    }
}

void DeleteCurdepth(struct SymbolTable* ST)
{
    //printf("DeleteCurdepth:%d\n",ST->curdepth);
	if(ST->curdepth>0)
	{
		struct Symbol* cur=ST->Stack[ST->curdepth],*next;
		while(cur!=NULL)
		{
            
			next=cur->stack_next;
			ST->HashTable[hash_pjw(cur->name)]=cur->hash_next;
            //printf("DELETING %s\n",cur->name);
			free(cur);
			cur=next;
		}
        ST->Stack[ST->curdepth--]=NULL;
	}
	else
	{
		printf("DELETE ERROR");
	}
}

void PrintST(struct SymbolTable* ST)
{
    printf("-----------------------------------\n");
    for(int i=0;i<TABLESIZE;i++)
    {
        struct Symbol* cur=ST->HashTable[i];
        while(cur!=NULL)
        {
            printf("%s\n",cur->name);
            cur=cur->hash_next;
        }
    }
    printf("-----------------------------------\n");
}

void SymbolInsert(struct SymbolTable* ST,struct Symbol* s)
{
	// if(SymbolFind(ST,s->name)!=NULL)
	// {
	// 	printf("INSERT ERROR");
	// 	return;
	// }
	// else
	// {
		s->hash_next=ST->HashTable[hash_pjw(s->name)];
		ST->HashTable[hash_pjw(s->name)]=s;

		s->stack_next=ST->Stack[ST->curdepth];
		ST->Stack[ST->curdepth]=s;
		return;
	//}
}

struct Symbol* SymbolFind(struct SymbolTable* ST,char* name)
{
	struct Symbol* temp=ST->HashTable[hash_pjw(name)];
	while(temp!=NULL&&strcmp(temp->name,name)!=0)
	{
		temp=temp->hash_next;
	}
	return temp;
}

struct Symbol* SymbolFindCurDepth(struct SymbolTable* ST,char* name)
{
    struct Symbol* cur=ST->Stack[ST->curdepth];
    while(cur!=NULL && strcmp(name,cur->name)!=0 )
    {
        cur=cur->stack_next;
    }
    return cur;
}

void senmatic_check(struct TreeNode* root)
{
    program(root);
}

void program(struct TreeNode* root)
{
    extdeflist(root->childlist[0]);
}

void extdeflist(struct TreeNode* root)
{
    //printf("ExtDefList\n");
    if(root->childnum>0)
    {
        extdef(root->childlist[0]);
        extdeflist(root->childlist[1]);
    }
}

void extdef(struct TreeNode* root)
{
    //printf("ExtDef\n");
    Type t=specifier(root->childlist[0]);
    if(strcmp(root->childlist[1]->name,"ExtDecList")==0)//ExtDef → Specifier ExtDecList SEMI
    {
        //printf("ExtDef → Specifier ExtDecList SEMI\n");
        extdeclist(t,root->childlist[1]);
    }
    else if(strcmp(root->childlist[1]->name,"SEMI")==0)//Specifier SEMI
    {
        //printf("ExtDef → Specifier SEMI\n");
    }
    else //Specifier FunDec CompSt
    {
        //printf("ExtDef → Specifier FunDec CompSt\n");
        ST.curdepth++;
        struct Symbol* S=fundec(t,root->childlist[1]);
        if(S==NULL) return;
        compst(S,root->childlist[2]);
        //PrintST(&ST);
        DeleteCurdepth(&ST);
        
    }
}

Type specifier(struct TreeNode* root)
{
    if(strcmp(root->childlist[0]->name,"TYPE")==0)
    {
        //printf("%s",root->childlist[0]->extra);
        Type t=malloc(sizeof(struct Type_));
        t->kind=BASIC;
        if(strcmp(root->childlist[0]->extra,"int")==0)
        {
            t->u.basic=0;//0-int 1-float
        }
        else if(strcmp(root->childlist[0]->extra,"float")==0)
        {
            t->u.basic=1;//0-int 1-float
        }
        else
        {
            printf("specifier basic ERROR");
        }
        return t;
    }
    else if(strcmp(root->childlist[0]->name,"StructSpecifier")==0)
    {
        Type t=structspecifier(root->childlist[0]);
        return t;
    }
}

Type structspecifier(struct TreeNode* root)
{
    ST.curdepth++;
    //printf("%d\n",ST.curdepth);
    if(root->childnum>2)//StructSpecifier → STRUCT OptTag LC DefList RC
    {
        Type t=malloc(sizeof(struct Type_));
        //printf("%d\n",(int)t);
        t->kind=STRUCTURE;
        t->u.structure=deflist_struct(root->childlist[3]);

        if(root->childlist[1]->childnum==0)//OptTag -> e
        {
            return t;
        }
        else//OptTag → ID
        {
            struct Symbol* pos = SymbolFind(&ST,root->childlist[1]->childlist[0]->extra);
            if(pos==NULL)
            {
                struct Symbol* s=malloc(sizeof(struct Symbol));
                s->name=root->childlist[1]->childlist[0]->extra;
                s->type=t;
                SymbolInsert(&ST,s);
                //printf("insert struct:%s\n",s->name);
                //PrintST(&ST);
                return t;
            }
            else//结构体的名字与前面定义过的结构体或变量的名字重复
            {
                fprintf(stderr, "Error type 16 at Line %d:%s\n",root->childlist[1]->line,"Duplicated name");
            }
        }
    }
    else//StructSpecifier -> STRUCT Tag
    {
        struct Symbol* pos = SymbolFind(&ST,root->childlist[1]->childlist[0]->extra);
        //printf("%s",root->childlist[1]->childlist[0]->extra);
        if(pos==NULL)
        {
            //PrintST(&ST);
            fprintf(stderr, "Error type 17 at Line %d:%s\n",root->childlist[1]->line,"Undefined structure");
            return NULL;
        }
        else
        {
            return pos->type;
        }
    }
    DeleteCurdepth(&ST);
}

FieldList deflist_struct(struct TreeNode* root)//DefList → Def DefList| e
{
    if(root->childnum==0) return NULL;
    else
    {
        FieldList temp=def_struct(root->childlist[0]);
        temp->tail=deflist_struct(root->childlist[1]);
    }
}

FieldList def_struct(struct TreeNode* root)//Def → Specifier DecList SEMI
{

    Type t=specifier(root->childlist[0]);
    return declist_struct(t,root->childlist[1]);
}

FieldList declist_struct(Type t,struct TreeNode* root)//DecList → Dec| Dec COMMA DecList
{
    //printf("%d\n",(int)struct_type->u.structure);
    FieldList FL=malloc(sizeof(struct FieldList_));
    FL->type=t;
    if(root->childnum==1)//DecList → Dec
    {
        dec_struct(FL,t,root->childlist[0]);
        FL->tail=NULL;
    }
    else//Dec COMMA DecList
    {
        dec_struct(FL,t,root->childlist[0]);
        FL->tail=declist_struct(t,root->childlist[2]);
    }
    return FL;
}

void dec_struct(FieldList FL,Type t,struct TreeNode* root)//Dec → VarDec | VarDec ASSIGNOP Exp
{
    struct TreeNode* r=root->childlist[0];
    while(r->childnum!=1)//VarDec → VarDec LB INT RB
    {
        Type temp_t=malloc(sizeof(struct Type_));
        temp_t->kind=ARRAY;
        temp_t->u.array.elem=FL->type;
        temp_t->u.array.size=r->childlist[2]->INT;

        FL->type=temp_t;
        // FL->type->kind=ARRAY;
        // FL->type->u.array.elem=FL->type;
        // FL->type->u.array.size=r->childlist[2]->INT;
        //printf("%d\n",FL->type->u.array.size);
        r=r->childlist[0];
    }

    FL->name=r->childlist[0]->extra;//VarDec → ID

    struct Symbol* S=malloc(sizeof(struct Symbol));
    S->name=FL->name;
    S->type=FL->type;

    if(SymbolFindCurDepth(&ST,S->name)==NULL)
    {
        SymbolInsert(&ST,S);
    }
    else
    {
        fprintf(stderr, "Error type 15 at Line %d:%s\n",root->childlist[0]->line,"Redefined variable");
    }


    // int flag=0;
    // FieldList cur = struct_type->u.structure;
    // printf("%d\n",(int)struct_type->u.structure);
    // while(cur!=NULL)
    // {
    //     printf("1");
    //     if(strcmp(cur->name,FL->name)==0)
    //     {
    //         fprintf(stderr, "Error type 15 at Line %d:%s\n",root->childlist[1]->line,"Redefined variable");
    //         break;
    //     }
    //     cur=cur->tail;
    // }
    //printf("%s\n",FL->name);

    if(root->childnum==1)//Dec → VarDec
    {
        
    }
    else//Dec → VarDec ASSIGNOP Exp
    {
        fprintf(stderr, "Error type 15 at Line %d:%s\n",root->childlist[1]->line,"Incorrect Initialization");
    }
}

void extdeclist(Type t,struct TreeNode* root)//ExtDecList → VarDec | VarDec COMMA ExtDecList
{
    if(root->childnum==1)//ExtDecList → VarDec
    {
        vardec(t,root->childlist[0]);
    }
    else//ExtDecList → VarDec COMMA ExtDecList
    {
        vardec(t,root->childlist[0]);
        extdeclist(t,root->childlist[2]);
    }
}

struct Symbol* vardec(Type t,struct TreeNode* root)//VarDec → ID | VarDec LB INT RB
{
    struct Symbol* S=malloc(sizeof(struct Symbol));
    //FieldList FL=malloc(sizeof(struct FieldList_));
    //FL->type=t;
    S->type=t;
    // if(root->childnum==1)
    // {
    //     //S->type=t;
    //     S->name=root->childlist[0]->extra;
    //     SymbolInsert(&ST,S);
    // }
    // else
    // {
        while(root->childnum!=1)
        {
            Type temp_t=malloc(sizeof(struct Type_));
            temp_t->kind=ARRAY;
            temp_t->u.array.elem=S->type;
            temp_t->u.array.size=root->childlist[2]->INT;

            S->type=temp_t;
            root=root->childlist[0];
        }
        S->name=root->childlist[0]->extra;
        if(SymbolFind(&ST,S->name)==NULL)
        {
            SymbolInsert(&ST,S);
            
        }
        else
        {
            fprintf(stderr, "Error type 3 at Line %d:%s\n",root->childlist[0]->line,"Redefined variable");
        }
        return S;
    //  s}
}


struct Symbol* fundec(Type ret_t,struct TreeNode* root)//FunDec → ID LP VarList RP | ID LP RP
{
    //printf("fundec\n");
    struct Symbol* S=malloc(sizeof(struct Symbol));
    Type t=malloc(sizeof(struct Type_));
    S->type=t;
    t->kind=FUNCTION;
    t->u.function.ret_type=ret_t;

    S->name=root->childlist[0]->extra;
    
    if(root->childnum==3)//FunDec → ID LP RP
    {
        t->u.function.num=0;
        t->u.function.para_list=NULL;
    }
    else
    {
        varlist(t,root->childlist[2]);
    }
    //printf("Finish fundec\n");
    if(SymbolFind(&ST,S->name)!=NULL)
    {
        fprintf(stderr, "Error type 4 at Line %d:%s\n",root->childlist[0]->line,"Redefined function");
        return NULL;
    }
    else
    {
        ST.curdepth--;
        SymbolInsert(&ST,S);
        ST.curdepth++;
        return S;
    }

    // FieldList temp=t->u.function.para_list;
    // for(int i=0;i<t->u.function.num;i++)
    // {
    //     printf("%s %d\n",temp->name,temp->type->kind);
    //     temp=temp->tail;
    // }
}

void varlist(Type t,struct TreeNode* root)//t:function
//VarList → ParamDec COMMA VarList | ParamDec
{
    //printf("varlist\n");
    t->u.function.num++;
    // Type* para_type_list=malloc(t->u.function.num*sizeof(Type));
    // for(int i=0;i<t->u.function.num-1;i++)
    // {
    //     para_type_list[i]=t->u.function.para_type_list[i];
    // }
    // free(t->u.function.para_type_list);
    // t->u.function.para_type_list=para_type_list;

    //ParamDec → Specifier VarDec
    FieldList FL=malloc(sizeof(struct FieldList_));
    FL->tail=NULL;
    FieldList cur=t->u.function.para_list;
    if(cur==NULL)
    {
        t->u.function.para_list=FL;
    }
    else
    {
        while(cur->tail!=NULL) cur=cur->tail;
        cur->tail=FL;
    }
    
    FL->type=specifier(root->childlist[0]->childlist[0]);
    struct TreeNode* VD=root->childlist[0]->childlist[1];
    while(VD->childnum!=1)//VarDec → ID | VarDec LB INT RB
    {
        Type temp_t=malloc(sizeof(struct Type_));
        temp_t->kind=ARRAY;
        temp_t->u.array.elem=FL->type;
        temp_t->u.array.size=VD->childlist[2]->INT;

        FL->type=temp_t;
        VD=VD->childlist[0];
    }
    FL->name=VD->childlist[0]->extra;
    struct Symbol* S=malloc(sizeof(struct Symbol));
    S->name=FL->name;
    S->type=FL->type;
    SymbolInsert(&ST,S);
    if(root->childnum>1)//VarList → ParamDec COMMA VarList 
    {
        varlist(t,root->childlist[2]);
    }
}

void compst(struct Symbol* S,struct TreeNode* root)
//CompSt → LC DefList StmtList RC
{
    //printf("compst\n");
    ST.curdepth++;
    deflist_func(root->childlist[1]);
    //printf("HHHHHHHHHHHHHHHHHHH\n");
    stmtlist(S,root->childlist[2]);
    DeleteCurdepth(&ST);
    //printf("Finish compst\n");
}

FieldList deflist_func(struct TreeNode* root)//DefList → Def DefList| e
{
    //printf("deflist_function\n");
    if(root->childnum==0) return NULL;
    else
    {
        def_func(root->childlist[0]);
        deflist_func(root->childlist[1]);
    }
}

void def_func(struct TreeNode* root)//Def → Specifier DecList SEMI
{
    //printf("def_function\n");
    Type t=specifier(root->childlist[0]);
    declist_func(t,root->childlist[1]);
}

void declist_func(Type t,struct TreeNode* root)
//DecList → Dec | Dec COMMA DecList
{
    //printf("declist in function\n");
    dec_func(t,root->childlist[0]);
    if(root->childnum>1)
    {
        declist_func(t,root->childlist[2]);
    }
}

void dec_func(Type t,struct TreeNode* root)
//Dec → VarDec | VarDec ASSIGNOP Exp
{
    //printf("dec_func\n");
    struct Symbol* S= vardec(t,root->childlist[0]);
    if(root->childnum>1)//VarDec ASSIGNOP Exp
    {
        Type exp_type=expp(root->childlist[2]);
        if(Type_equal(exp_type,S->type)==0)
        {
            fprintf(stderr, "Error type 5 at Line %d:%s\n",root->childlist[1]->line," Type mismatched for assignment");
        }
    }
}

void stmtlist(struct Symbol* S, struct TreeNode* root)//StmtList → Stmt StmtList | e
{
    //printf("stmtlist\n");
    if(root->childnum!=0)
    {
        stmt(S,root->childlist[0]);
        stmtlist(S,root->childlist[1]);
    }
}

void stmt(struct Symbol* S, struct TreeNode* root)
// Stmt → Exp SEMI
// | CompSt
// | RETURN Exp SEMI
// | IF LP Exp RP Stmt
// | IF LP Exp RP Stmt ELSE Stmt
// | WHILE LP Exp RP Stmt
{
    //printf("Stmt\n");
    //printf("%s\n",root->childlist[0]->name);
    if(strcmp(root->childlist[0]->name,"Exp")==0)
    {
        //printf("Stmt → Exp SEMI\n");
        expp(root->childlist[0]);
    }
    else if(strcmp(root->childlist[0]->name,"CompSt")==0)
    {
        compst(S,root->childlist[0]);
    }
    else if(strcmp(root->childlist[0]->name,"RETURN")==0)
    {
        Type t=expp(root->childlist[1]);
        //printf("%d\n",S->type->kind);
        if(S->type->kind==FUNCTION && Type_equal(S->type->u.function.ret_type,t)==1)
        {
            //printf("JJJJ\n");
            //return t;
        }
        else
        {
            //printf("KKKK\n");
            fprintf(stderr, "Error type 8 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for return.");
            //return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"IF")==0&&root->childnum==5)
    {
        Type t=expp(root->childlist[2]);
        if(t->kind=BASIC && t->u.basic==INTT)
        {

        }
        else
        {
            printf("error:if condition\n");
        }
    }
    else if(strcmp(root->childlist[0]->name,"IF")==0&&root->childnum==7)
    {
        Type t=expp(root->childlist[2]);
        if(t->kind=BASIC && t->u.basic==INTT)
        {

        }
        else
        {
            printf("error:if condition\n");
        }
    }
    else//WHILE LP Exp RP Stmt
    {
        Type t=expp(root->childlist[2]);
        if(t->kind=BASIC && t->u.basic==INTT)
        {

        }
        else
        {
            printf("error:while condition\n");
        }
    }
}

Type expp(struct TreeNode* root)
// Exp → 
//   Exp ASSIGNOP Exp
// | Exp AND Exp
// | Exp OR Exp
// | Exp RELOP Exp
// | Exp PLUS Exp
// | Exp MINUS Exp
// | Exp STAR Exp
// | Exp DIV Exp

// | LP Exp RP
// | MINUS Exp
// | NOT Exp
// | ID LP Args RP
// | ID LP RP

// | Exp LB Exp RB
// | Exp DOT ID

// | ID
// | INT
// | FLOAT
{
    //printf("exp\n");
    //printf("%s\n",root->childlist[0]->name);
    if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"ASSIGNOP")==0)
    {
        //printf("%s\n",root->childlist[0]->childlist[0]->name);
        if(   (strcmp(root->childlist[0]->childlist[0]->name,"ID")==0)
            ||(strcmp(root->childlist[0]->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[0]->childlist[1]->name,"LB")==0)
            ||(strcmp(root->childlist[0]->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[0]->childlist[1]->name,"DOT")==0)
        )
        {
            Type t1 = expp(root->childlist[0]);
            Type t2 = expp(root->childlist[2]);
            if(t1==NULL || t2==NULL) return NULL;

            if(Type_equal(t1,t2)==1)
            {
                return t1;
            }
            else
            {
                fprintf(stderr, "Error type 5 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for assignment");
                return NULL;
            }
        }
        else
        {
            fprintf(stderr, "Error type 6 at Line %d:%s\n",root->childlist[0]->line,"The left-hand side of an assignment must be a variable");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"AND")==0)
    {
        Type t1 = expp(root->childlist[0]);
        Type t2 = expp(root->childlist[2]);
        if(t1==NULL || t2==NULL) return NULL;

        if(t1->kind==BASIC && t1->u.basic==INTT && Type_equal(t1,t2)==1)
        {
            return t1;
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"OR")==0)
    {
        Type t1 = expp(root->childlist[0]);
        Type t2 = expp(root->childlist[2]);
        if(t1==NULL || t2==NULL) return NULL;

        if(t1->kind==BASIC && t1->u.basic==INTT && Type_equal(t1,t2)==1)
        {
            return t1;
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"RELOP")==0)
    {
        Type t1 = expp(root->childlist[0]);
        Type t2 = expp(root->childlist[2]);
        if(t1==NULL || t2==NULL) return NULL;

        if(t1->kind==BASIC && Type_equal(t1,t2)==1)
        {
            return t1;
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"PLUS")==0)
    {
        Type t1 = expp(root->childlist[0]);
        Type t2 = expp(root->childlist[2]);
        if(t1==NULL || t2==NULL) return NULL;

        if(t1->kind==BASIC && Type_equal(t1,t2)==1)
        {
            return t1;
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"MINUS")==0)
    {
        Type t1 = expp(root->childlist[0]);
        Type t2 = expp(root->childlist[2]);
        if(t1==NULL || t2==NULL) return NULL;

        if(t1->kind==BASIC && Type_equal(t1,t2)==1)
        {
            return t1;
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"STAR")==0)
    {
        Type t1 = expp(root->childlist[0]);
        Type t2 = expp(root->childlist[2]);
        if(t1==NULL || t2==NULL) return NULL;
        if(t1->kind==BASIC && Type_equal(t1,t2)==1)
        {
            return t1;
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"DIV")==0)
    {
        Type t1 = expp(root->childlist[0]);
        Type t2 = expp(root->childlist[2]);
        if(t1==NULL || t2==NULL) return NULL;
        if(t1->kind==BASIC && Type_equal(t1,t2)==1)
        {
            return t1;
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }

    else if(strcmp(root->childlist[0]->name,"LP")==0)//LP Exp RP
    {
        return expp(root->childlist[1]);
    }
    else if(strcmp(root->childlist[0]->name,"MINUS")==0)//MINUS Exp
    {
        Type t = expp(root->childlist[1]);
        if(t==NULL) return NULL;
        if(t->kind==BASIC)
        {
            return t;
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"NOT")==0)//NOT Exp
    {
        Type t = expp(root->childlist[1]);
        if(t==NULL) return NULL;
        if(t->kind==BASIC && t->u.basic==INTT)
        {
            return expp(root->childlist[1]);
        }
        else
        {
            fprintf(stderr, "Error type 7 at Line %d:%s\n",root->childlist[0]->line,"Type mismatched for operands");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"ID")==0 && root->childnum==4)//ID LP Args RP  //Args → Exp COMMA Args | Exp
    {
        struct Symbol* S=SymbolFind(&ST,root->childlist[0]->extra);
        if(S==NULL)
        {
            fprintf(stderr, "Error type 2 at Line %d:%s\n",root->childlist[0]->line,"Undefined function");
            return NULL;
        }
        if(S->type->kind!=FUNCTION)
        {
            fprintf(stderr, "Error type 11 at Line %d:%s\n",root->childlist[0]->line,"not a function");
            return NULL;
        }
        if(check_funpara(S->type->u.function.para_list,root->childlist[2])==1) return S->type;
        else
        {
            fprintf(stderr, "Error type 9 at Line %d:%s\n",root->childlist[0]->line,"arguments do not suit function");
            return NULL;
        }
    }
    else if(strcmp(root->childlist[0]->name,"ID")==0 && root->childnum==3)//ID LP RP
    {
        struct Symbol* S=SymbolFind(&ST,root->childlist[0]->extra);
        if(S==NULL)
        {
            fprintf(stderr, "Error type 2 at Line %d:%s\n",root->childlist[0]->line,"Undefined function");
            return NULL;
        }
        if(S->type->kind!=FUNCTION)
        {
            fprintf(stderr, "Error type 11 at Line %d:%s\n",root->childlist[0]->line,"not a function");
            return NULL;
        }
        if(S->type->u.function.para_list==NULL) return S->type;
        else
        {
            fprintf(stderr, "Error type 9 at Line %d:%s\n",root->childlist[0]->line,"arguments do not suit function");
            return NULL;
        }
    }

    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"LB")==0)//Exp LB Exp RB
    {
        Type t1=expp(root->childlist[0]);
        Type t2=expp(root->childlist[2]);
        if(t1==NULL||t2==NULL) return NULL;
        if(t1->kind!=ARRAY)
        {
            fprintf(stderr, "Error type 10 at Line %d:%s\n",root->childlist[0]->line,"not an array");
            return NULL;
        }
        if(t2->kind!=BASIC || (t2->kind==BASIC && t2->u.basic!=INTT))
        {
            fprintf(stderr, "Error type 12 at Line %d:%s\n",root->childlist[2]->line,"not an integer");
            return NULL;
        }
        return t1->u.array.elem;
    }
    else if(strcmp(root->childlist[0]->name,"Exp")==0 && strcmp(root->childlist[1]->name,"DOT")==0)//Exp DOT ID
    {
        Type t=expp(root->childlist[0]);
        if(t==NULL) return NULL;
        if(t->kind==STRUCTURE)
        {
            FieldList FL=check_struct(t->u.structure,root->childlist[2]->extra);
            if(FL==NULL)
            {
                fprintf(stderr, "Error type 14 at Line %d:%s\n",root->childlist[2]->line,"Non-existent field");
                return NULL;
            }
            else
            {
                return FL->type;
            }
        }
        else
        {
            fprintf(stderr, "Error type 13 at Line %d:%s\n",root->childlist[0]->line,"Illegal use of \".\"");
            return NULL;
        }
    }

    else if(strcmp(root->childlist[0]->name,"ID")==0 && root->childnum==1)
    {
        //printf("exp -> ID\n");
        struct Symbol* S=SymbolFind(&ST,root->childlist[0]->extra);
        if(S==NULL)
        {
            fprintf(stderr, "Error type 1 at Line %d:%s\n",root->childlist[0]->line,"Undefined variable");
            return NULL;
        }
        else
        {
            //printf("%d\n",S->type->kind);
            return S->type;
        }
    }
    else if(strcmp(root->childlist[0]->name,"INT")==0)
    {
        //printf("exp -> INT\n");
        Type t=malloc(sizeof(struct Type_));
        t->kind=BASIC;
        t->u.basic=INTT;
        return t;
    }
    else if(strcmp(root->childlist[0]->name,"FLOAT")==0)
    {
        //printf("exp -> FLOAT\n");
        Type t=malloc(sizeof(struct Type_));
        t->kind=BASIC;
        t->u.basic=FLOATT;
        return t;
    }
}

FieldList check_struct(FieldList t,char* name)//1-in 0-not in
{
    if(t!=NULL)
    {
        if(strcmp(t->name,name)==0)
        {
            return t;
        }
        else
        {
            return check_struct(t->tail,name);
        }
    }
    else
    {
        return NULL;
    }
}

int check_funpara(FieldList FL,struct TreeNode* root)//Args → Exp COMMA Args | Exp
{
    if(FL==NULL&&root==NULL) return 1;
    if(FL==NULL||root==NULL) return 0;
    if(Type_equal(FL->type,expp(root->childlist[0]))==1)
    {
        return check_funpara(FL->tail,root->childlist[2]);
    }
    else
    {
        return 0;
    }
}