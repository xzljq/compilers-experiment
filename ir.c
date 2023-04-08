#include "ir.h"

//extern struct TreeNode* root;
extern struct SymbolTable ST;

struct InterCodes* IC_head = NULL;
int temp_count=0;
int lable_count=0;

void DeleteIR(struct InterCodes* head)
{
    if(head!=NULL)
    {
        if(head->prev==NULL && head->next==NULL)
        {
            printf("DELETE IR head\n");
            exit(0);
            head->prev->next=NULL;
        }
        else if(head->prev!=NULL && head->next==NULL)
        {
            head->prev->next=NULL;
        }
        else if(head->prev==NULL && head->next!=NULL)
        {
            printf("DELETE IR head\n");
            exit(0);
        }
        else
        {
            head->prev->next=head->next;
        }
    }
}

void ProcessIR(struct InterCodes* head)
{
    while(head!=NULL)
    {
        if(head->code.kind==IC_ASSIGN)
        {
            if(head->code.u.assign.left==NULL)
            {
                DeleteIR(head);
                head=head->next;
                continue;
            }
        }
        head=head->next;
    }
}

void Print_IR(struct InterCodes* head)
{
    while(head!=NULL)
    {
        //printf("%d\n",head->code.kind);
        #ifdef _DEBUG
        printf("%d %d\t",head->code.BasicBlock,head->code.InternalNo);
        #endif
        if(head->code.kind==IC_ASSIGN)
        {
            if(head->code.u.assign.left==NULL)
            {
                #ifdef _DEBUG
                printf("left is NULL\n");
                #endif
                head=head->next;
                exit(0);
                continue;
            }
            #ifdef _DEBUG
            print_OP(head->code.u.assign.left);
            printf(" := ");
            print_OP(head->code.u.assign.right);
            #endif
        }
        else if(head->code.kind==IC_ADD)
        {
            #ifdef _DEBUG
            
            print_OP(head->code.u.binop.result);
		    printf(" := ");
			print_OP(head->code.u.binop.op1);
			printf(" + ");
			print_OP(head->code.u.binop.op2);
            #endif
        }
        else if(head->code.kind==IC_SUB)
        {
            print_OP(head->code.u.binop.result);
		    printf(" := ");
			print_OP(head->code.u.binop.op1);
			printf(" - ");
			print_OP(head->code.u.binop.op2);
        }
        else if(head->code.kind==IC_MUL)
        {
            print_OP(head->code.u.binop.result);
		    printf(" := ");
			print_OP(head->code.u.binop.op1);
			printf(" * ");
			print_OP(head->code.u.binop.op2);
        }
        else if(head->code.kind==IC_DIV)
        {
            print_OP(head->code.u.binop.result);
		    printf(" := ");
			print_OP(head->code.u.binop.op1);
			printf(" / ");
			print_OP(head->code.u.binop.op2);
        }
        else if(head->code.kind==IC_LABLE)
        {
            printf("LABEL ");
			print_OP(head->code.u.lable.op_lable);
            printf(" :");
        }
        else if(head->code.kind==IC_FUNC)
        {
            printf("FUNCTION ");
			print_OP(head->code.u.func.op_func);
            printf(" :");
        }
        else if(head->code.kind==IC_RETURN)
        {
            printf("RETURN ");
			print_OP(head->code.u.ret.op_return);
        }
        else if(head->code.kind==IC_GOTO)
        {
            printf("GOTO ");
			print_OP(head->code.u.gt.op_goto);
        }
        else if(head->code.kind==IC_IFGOTO)
        {
            printf("IF ");
			print_OP(head->code.u.ifgt.x);
            printf(" ");
            print_OP(head->code.u.ifgt.relop);
            printf(" ");
            print_OP(head->code.u.ifgt.y);
            printf(" GOTO ");
            print_OP(head->code.u.ifgt.z);
        }
        else if(head->code.kind==IC_READ)
        {
            if(head->code.u.read.op_read==NULL)
            {
                head->code.u.read.op_read=new_temp();
            }
            #ifdef _DEBUG
            printf("READ ");
			print_OP(head->code.u.read.op_read);
            #endif

        }
        else if(head->code.kind==IC_WRITE)
        {
            #ifdef _DEBUG
            printf("WRITE ");
			print_OP(head->code.u.write.op_write);
            #endif

        }
        else if(head->code.kind==IC_CALL)
        {
            if(head->code.u.call.x==NULL)
            {
                #ifdef _DEBUG
                printf("ADD A USELESS FUNC RET\n");
                #endif
                head->code.u.call.x=new_temp();
            }
            #ifdef _DEBUG
            print_OP(head->code.u.call.x);
            printf(" := CALL ");
			print_OP(head->code.u.call.f);
            #endif

        }
        else if(head->code.kind==IC_ARG)
        {
            #ifdef _DEBUG
            printf("ARG ");
			print_OP(head->code.u.arg.x);
            #endif
        }
        else if(head->code.kind==IC_DEC)
        {
            #ifdef _DEBUG
            printf("DEC ");
			print_OP(head->code.u.dec.x);
            printf(" [%d]",head->code.u.dec.size);
            #endif
        }
        else if(head->code.kind==IC_PARAM)
        {
            #ifdef _DEBUG
            printf("PARAM ");
			print_OP(head->code.u.param.x);
            #endif
        }
        else if(head->code.kind==IC_ADDR)
        {
            #ifdef _DEBUG
            print_OP(head->code.u.addr.left);
            printf(" := &");
            print_OP(head->code.u.addr.right);
            #endif
        }
        else if(head->code.kind==IC_R_DEREF)
        {
            #ifdef _DEBUG
            print_OP(head->code.u.r_deref.left);
            printf(" := *");
            print_OP(head->code.u.r_deref.right);
            #endif
        }
        else if(head->code.kind==IC_L_DEREF)
        {
            printf("*");
            print_OP(head->code.u.l_deref.left);
            printf(" := ");
            print_OP(head->code.u.l_deref.right);
        }
        else
        {
            printf("IC PRINT ERROR!\n");
        }
        printf("\n");
        head=head->next;
    }
}

void writeToFile(struct InterCodes* head,FILE* f)
{
    while(head!=NULL)
    {
        //printf("%d\n",head->code.kind);
        #ifdef _DEBUG
        printf("%d\t",head->code.BasicBlock);
        #endif
        if(head->code.kind==IC_ASSIGN)
        {
            if(head->code.u.assign.left==NULL)
            {
                #ifdef _DEBUG
                printf("left is NULL\n");
                #endif
                head=head->next;
                exit(0);
                continue;
            }
            #ifdef _DEBUG
            print_OP(head->code.u.assign.left);
            printf(" := ");
            print_OP(head->code.u.assign.right);
            #endif

            write_OP(head->code.u.assign.left,f);
            fprintf(f," := ");
            write_OP(head->code.u.assign.right,f);
        }
        else if(head->code.kind==IC_ADD)
        {
            #ifdef _DEBUG
            
            print_OP(head->code.u.binop.result);
		    printf(" := ");
			print_OP(head->code.u.binop.op1);
			printf(" + ");
			print_OP(head->code.u.binop.op2);
            #endif

            write_OP(head->code.u.binop.result,f);
		    fprintf(f, " := ");
			write_OP(head->code.u.binop.op1,f);
			fprintf(f, " + ");
			write_OP(head->code.u.binop.op2,f);
        }
        else if(head->code.kind==IC_SUB)
        {
            #ifdef _DEBUG
            print_OP(head->code.u.binop.result);
		    printf(" := ");
			print_OP(head->code.u.binop.op1);
			printf(" - ");
			print_OP(head->code.u.binop.op2);
            #endif

            write_OP(head->code.u.binop.result,f);
		    fprintf(f, " := ");
			write_OP(head->code.u.binop.op1,f);
			fprintf(f, " - ");
			write_OP(head->code.u.binop.op2,f);
        }
        else if(head->code.kind==IC_MUL)
        {
            #ifdef _DEBUG
            print_OP(head->code.u.binop.result);
		    printf(" := ");
			print_OP(head->code.u.binop.op1);
			printf(" * ");
			print_OP(head->code.u.binop.op2);
            #endif

            write_OP(head->code.u.binop.result,f);
		    fprintf(f, " := ");
			write_OP(head->code.u.binop.op1,f);
			fprintf(f, " * ");
			write_OP(head->code.u.binop.op2,f);
        }
        else if(head->code.kind==IC_DIV)
        {
            #ifdef _DEBUG
            print_OP(head->code.u.binop.result);
		    printf(" := ");
			print_OP(head->code.u.binop.op1);
			printf(" / ");
			print_OP(head->code.u.binop.op2);
            #endif

            write_OP(head->code.u.binop.result,f);
		    fprintf(f, " := ");
			write_OP(head->code.u.binop.op1,f);
			fprintf(f, " / ");
			write_OP(head->code.u.binop.op2,f);
        }
        else if(head->code.kind==IC_LABLE)
        {
            #ifdef _DEBUG
            printf("LABEL ");
			print_OP(head->code.u.lable.op_lable);
            printf(" :");
            #endif

            fprintf(f, "LABEL ");
			write_OP(head->code.u.lable.op_lable,f);
            fprintf(f, " :");
        }
        else if(head->code.kind==IC_FUNC)
        {
            #ifdef _DEBUG
            printf("FUNCTION ");
			print_OP(head->code.u.func.op_func);
            printf(" :");
            #endif

            fprintf(f, "FUNCTION ");
			write_OP(head->code.u.func.op_func,f);
            fprintf(f, " :");
        }
        else if(head->code.kind==IC_RETURN)
        {
            #ifdef _DEBUG
            printf("RETURN ");
			print_OP(head->code.u.ret.op_return);
            #endif

            fprintf(f, "RETURN ");
			write_OP(head->code.u.ret.op_return,f);
        }
        else if(head->code.kind==IC_GOTO)
        {
            #ifdef _DEBUG
            printf("GOTO ");
			print_OP(head->code.u.gt.op_goto);
            #endif

            fprintf(f, "GOTO ");
			write_OP(head->code.u.gt.op_goto,f);
        }
        else if(head->code.kind==IC_IFGOTO)
        {
            #ifdef _DEBUG
            printf("IF ");
			print_OP(head->code.u.ifgt.x);
            printf(" ");
            print_OP(head->code.u.ifgt.relop);
            printf(" ");
            print_OP(head->code.u.ifgt.y);
            printf(" GOTO ");
            print_OP(head->code.u.ifgt.z);
            #endif

            fprintf(f, "IF ");
			write_OP(head->code.u.ifgt.x,f);
            fprintf(f, " ");
            write_OP(head->code.u.ifgt.relop,f);
            fprintf(f, " ");
            write_OP(head->code.u.ifgt.y,f);
            fprintf(f, " GOTO ");
            write_OP(head->code.u.ifgt.z,f);
        }
        else if(head->code.kind==IC_READ)
        {
            if(head->code.u.read.op_read==NULL)
            {
                head->code.u.read.op_read=new_temp();
            }
            #ifdef _DEBUG
            printf("READ ");
			print_OP(head->code.u.read.op_read);
            #endif

            fprintf(f, "READ ");
			write_OP(head->code.u.read.op_read,f);
        }
        else if(head->code.kind==IC_WRITE)
        {
            #ifdef _DEBUG
            printf("WRITE ");
			print_OP(head->code.u.write.op_write);
            #endif

            fprintf(f, "WRITE ");
			write_OP(head->code.u.write.op_write,f);
        }
        else if(head->code.kind==IC_CALL)
        {
            if(head->code.u.call.x==NULL)
            {
                #ifdef _DEBUG
                printf("ADD A USELESS FUNC RET\n");
                #endif
                head->code.u.call.x=new_temp();
            }
            #ifdef _DEBUG
            print_OP(head->code.u.call.x);
            printf(" := CALL ");
			print_OP(head->code.u.call.f);
            #endif

            write_OP(head->code.u.call.x,f);
            fprintf(f, " := CALL ");
			write_OP(head->code.u.call.f,f);
        }
        else if(head->code.kind==IC_ARG)
        {
            #ifdef _DEBUG
            printf("ARG ");
			print_OP(head->code.u.arg.x);
            #endif

            fprintf(f, "ARG ");
			write_OP(head->code.u.arg.x,f);
        }
        else if(head->code.kind==IC_DEC)
        {
            #ifdef _DEBUG
            printf("DEC ");
			print_OP(head->code.u.dec.x);
            printf(" [%d]",head->code.u.dec.size);
            #endif

            fprintf(f, "DEC ");
			write_OP(head->code.u.dec.x,f);
            fprintf(f, " %d",head->code.u.dec.size);
        }
        else if(head->code.kind==IC_PARAM)
        {
            #ifdef _DEBUG
            printf("PARAM ");
			print_OP(head->code.u.param.x);
            #endif

            fprintf(f, "PARAM ");
			write_OP(head->code.u.param.x,f);
        }
        else if(head->code.kind==IC_ADDR)
        {
            #ifdef _DEBUG
            print_OP(head->code.u.addr.left);
            printf(" := &");
            print_OP(head->code.u.addr.right);
            #endif

            write_OP(head->code.u.addr.left,f);
            fprintf(f," := &");
            write_OP(head->code.u.addr.right,f);
        }
        else if(head->code.kind==IC_R_DEREF)
        {
            #ifdef _DEBUG
            print_OP(head->code.u.r_deref.left);
            printf(" := *");
            print_OP(head->code.u.r_deref.right);
            #endif

            write_OP(head->code.u.r_deref.left,f);
            fprintf(f," := *");
            write_OP(head->code.u.r_deref.right,f);
        }
        else if(head->code.kind==IC_L_DEREF)
        {
            #ifdef _DEBUG
            printf("*");
            print_OP(head->code.u.l_deref.left);
            printf(" := ");
            print_OP(head->code.u.l_deref.right);
            #endif

            fprintf(f,"*");
            write_OP(head->code.u.l_deref.left,f);
            fprintf(f," := ");
            write_OP(head->code.u.l_deref.right,f);
        }
        else
        {
            printf("IC PRINT ERROR!\n");
        }
        fprintf(f,"\n");
        printf("\n");
        head=head->next;
    }
}

void write_OP(Operand op,FILE* f)
{
    switch (op->kind)
    {
    case VARIABLE:
        fprintf(f, "%s",op->u.var_name);
        break;
    case CONSTANT:
        fprintf(f, "#%d",op->u.value);
        break;
    case ADDRESS:
        fprintf(f, "%s",op->u.var_name);
        break;
    case LABLE:
        fprintf(f, "%s",op->u.lable_name);
        break;
    case REL:
        fprintf(f, "%s",op->u.relop);
        break;
    case FUNC:
        fprintf(f, "%s",op->u.func_name);
        break;
    default:
        break;
    }
}

void print_OP(Operand op)
{
    switch (op->kind)
    {
    case VARIABLE:
        printf("%s",op->u.var_name);
        break;
    case CONSTANT:
        printf("#%d",op->u.value);
        break;
    case ADDRESS:
        printf("%s",op->u.var_name);
        break;
    case LABLE:
        printf("%s",op->u.lable_name);
        break;
    case REL:
        printf("%s",op->u.relop);
        break;
    case FUNC:
        printf("%s",op->u.func_name);
        break;
    default:
        break;
    }
}

Operand new_temp()
{
    Operand temp=malloc(sizeof(struct Operand_));
    temp->kind=VARIABLE;
    temp->u.var_name=malloc(100);
    sprintf(temp->u.var_name,"t%d", temp_count++);
    return temp;
}

Operand new_lable()
{
    Operand lable=malloc(sizeof(struct Operand_));
    lable->kind=LABLE;
    lable->u.lable_name=malloc(100);
    sprintf(lable->u.lable_name,"lable%d", lable_count++);
    return lable;
}

struct InterCodes* new_code()
{
    struct InterCodes* code=malloc(sizeof(struct InterCodes));
    code->next=code->prev=NULL;
    return code;
}

Operand get_relop(struct TreeNode* relop)
{
    Operand op=malloc(sizeof(struct Operand_));
    op->kind=REL;
    op->u.relop=relop->extra;
    return op;
}

Operand new_constant(int value)
{
    Operand op=malloc(sizeof(struct Operand_));
    op->kind=CONSTANT;
    op->u.value=value;
    return op;
}

void connect_ic(struct InterCodes* code1,struct InterCodes* code2)
{
    //printf("connect %d %d\n",(int)code1,(int)code2);
    if(code1==NULL&&code2==NULL)
    {
        printf("INTERCODE CONNECT ERROR\n");
    }
    if(code1==NULL)
    {
        //printf("AAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
        code1=code2;
        return;
    }
    else if(code2==NULL)
    {
        return;
    }
    while(code1!=NULL && code1->next!=NULL)
        code1=code1->next;
    
    while(code2!=NULL && code2->prev!=NULL)
        code2=code2->prev;
        
    code1->next=code2;
    code2->prev=code1;
}

struct InterCodes* translate_Exp(struct TreeNode* Exp, struct SymbolTable* sym_table, Operand place)
{
    #ifdef _DEBUG
        printf("translate_Exp → ");
    #endif
    if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"ASSIGNOP")==0)
    {
        #ifdef _DEBUG
            printf("Exp ASSIGNOP Exp\n");
        #endif

        // Operand t1=new_temp();
        // Operand t2=new_temp();
        // struct InterCodes* code1=translate_Exp(Exp->childlist[0],sym_table,t1);
        // struct InterCodes* code2=translate_Exp(Exp->childlist[2],sym_table,t2);
        // struct InterCodes* code_assign=new_code();
        // code_assign->code.kind=IC_ASSIGN;
        // code_assign->code.u.assign.left=t1;
        // code_assign->code.u.assign.right=t2;


        

        if(strcmp(Exp->childlist[0]->childlist[0]->name,"ID")==0 && Exp->childlist[0]->childnum==1)//单个变量访问
        {
            struct Symbol* S = SymbolFind(&ST,Exp->childlist[0]->childlist[0]->extra);
            Operand t1=new_temp();
            struct InterCodes* code1=translate_Exp(Exp->childlist[2],&ST,t1);

            struct InterCodes* code2=new_code();
            code2->code.kind=IC_ASSIGN;
            Operand variable=malloc(sizeof(struct Operand_));
            variable->kind=VARIABLE;
            variable->u.var_name=Exp->childlist[0]->childlist[0]->extra;
            code2->code.u.assign.left=variable;
            code2->code.u.assign.right=t1;

            if(place!=NULL)
            {
                struct InterCodes* code3=new_code();
                code3->code.kind=IC_ASSIGN;
                code3->code.u.assign.left=place;
                code3->code.u.assign.right=variable;

                connect_ic(code2,code3);
            }

            connect_ic(code1,code2);
            
            return code1;
        }
        else if(strcmp(Exp->childlist[0]->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[0]->childlist[1]->name,"LB")==0)//数组元素访问
        {
            //1维数组
            Operand t1=new_temp();
            t1->kind=ADDRESS;
            struct InterCodes* code1=translate_Exp(Exp->childlist[0],sym_table,t1);

            Operand t2=new_temp();
            struct InterCodes* code2=translate_Exp(Exp->childlist[2],sym_table,t2);

            struct InterCodes* code3=new_code();
            code3->code.kind=IC_L_DEREF;
            code3->code.u.assign.left=t1;
            code3->code.u.assign.right=t2;

            connect_ic(code1,code2);
            connect_ic(code2,code3);
            return code1;
        }
        else//结构体特定域的访问
        {
            Operand t1=new_temp();
            t1->kind=ADDRESS;
            struct InterCodes* code1=translate_Exp(Exp->childlist[0],sym_table,t1);

            Operand t2=new_temp();
            struct InterCodes* code2=translate_Exp(Exp->childlist[2],sym_table,t2);

            struct InterCodes* code3=new_code();
            code3->code.kind=IC_L_DEREF;
            code3->code.u.assign.left=t1;
            code3->code.u.assign.right=t2;

            connect_ic(code1,code2);
            connect_ic(code2,code3);
            return code1;
        }
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"AND")==0)
    {
        #ifdef _DEBUG
            printf("Exp AND Exp\n");
        #endif
        Operand label1=new_lable();
        Operand label2=new_lable();

        struct InterCodes* code0=new_code();
        code0->code.kind=IC_ASSIGN;
        Operand variable=new_constant(0);
        code0->code.u.assign.right=variable;
        code0->code.u.assign.left=place;

        struct InterCodes* code1=translate_Cond(Exp, label1, label2, &ST);
        
        struct InterCodes* code2_1=new_code();
        code2_1->code.kind=IC_LABLE;
        code2_1->code.u.lable.op_lable=label1;

        struct InterCodes* code2_2=new_code();
        code2_2->code.kind=IC_ASSIGN;
        Operand variable2=new_constant(1);
        code2_2->code.u.assign.right=variable2;
        code2_2->code.u.assign.left=place;

        struct InterCodes* code3=new_code();
        code3->code.kind=IC_LABLE;
        code3->code.u.lable.op_lable=label2;

        connect_ic(code0,code1);
        connect_ic(code1,code2_1);
        connect_ic(code2_1,code2_2);
        connect_ic(code2_2,code3);

        return code0;
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"OR")==0)
    {
        #ifdef _DEBUG
            printf("Exp OR Exp\n");
        #endif
        Operand label1=new_lable();
        Operand label2=new_lable();

        struct InterCodes* code0=new_code();
        code0->code.kind=IC_ASSIGN;
        Operand variable=new_constant(0);
        code0->code.u.assign.right=variable;
        code0->code.u.assign.left=place;

        struct InterCodes* code1=translate_Cond(Exp, label1, label2, &ST);
        
        struct InterCodes* code2_1=new_code();
        code2_1->code.kind=IC_LABLE;
        code2_1->code.u.lable.op_lable=label1;

        struct InterCodes* code2_2=new_code();
        code2_2->code.kind=IC_ASSIGN;
        Operand variable2=new_constant(1);
        code2_2->code.u.assign.right=variable2;
        code2_2->code.u.assign.left=place;

        struct InterCodes* code3=new_code();
        code3->code.kind=IC_LABLE;
        code3->code.u.lable.op_lable=label2;

        connect_ic(code0,code1);
        connect_ic(code1,code2_1);
        connect_ic(code2_1,code2_2);
        connect_ic(code2_2,code3);

        return code0;
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"RELOP")==0)
    {
        #ifdef _DEBUG
            printf("Exp RELOP Exp\n");
        #endif
        Operand label1=new_lable();
        Operand label2=new_lable();

        struct InterCodes* code0=new_code();
        code0->code.kind=IC_ASSIGN;
        Operand variable=new_constant(0);
        code0->code.u.assign.right=variable;
        code0->code.u.assign.left=place;

        struct InterCodes* code1=translate_Cond(Exp, label1, label2, &ST);
        
        struct InterCodes* code2_1=new_code();
        code2_1->code.kind=IC_LABLE;
        code2_1->code.u.lable.op_lable=label1;

        struct InterCodes* code2_2=new_code();
        code2_2->code.kind=IC_ASSIGN;
        Operand variable2=new_constant(1);
        code2_2->code.u.assign.right=variable2;
        code2_2->code.u.assign.left=place;

        struct InterCodes* code3=new_code();
        code3->code.kind=IC_LABLE;
        code3->code.u.lable.op_lable=label2;

        connect_ic(code0,code1);
        connect_ic(code1,code2_1);
        connect_ic(code2_1,code2_2);
        connect_ic(code2_2,code3);

        return code0;
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"PLUS")==0)
    {
        #ifdef _DEBUG
            printf("Exp PLUS Exp\n");
        #endif

        Operand t1=new_temp();
        Operand t2=new_temp();
        struct InterCodes* code1=translate_Exp(Exp->childlist[0],&ST,t1);
        struct InterCodes* code2=translate_Exp(Exp->childlist[2],&ST,t2);

        if(place!=NULL)
        {
            struct InterCodes* code3=new_code();
            code3->code.kind=IC_ADD;
            code3->code.u.binop.op1=t1;
            code3->code.u.binop.op2=t2;
            code3->code.u.binop.result=place;

            connect_ic(code2,code3);
        }
        

        connect_ic(code1,code2);
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"MINUS")==0)
    {
        #ifdef _DEBUG
            printf("Exp MINUS Exp\n");
        #endif
        Operand t1=new_temp();
        Operand t2=new_temp();
        struct InterCodes* code1=translate_Exp(Exp->childlist[0],&ST,t1);
        struct InterCodes* code2=translate_Exp(Exp->childlist[2],&ST,t2);

        if(place!=NULL)
        {
            struct InterCodes* code3=new_code();
            code3->code.kind=IC_SUB;
            code3->code.u.binop.op1=t1;
            code3->code.u.binop.op2=t2;
            code3->code.u.binop.result=place;

            connect_ic(code2,code3);
        }

        connect_ic(code1,code2);
        
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"STAR")==0)
    {
        #ifdef _DEBUG
            printf("Exp STAR Exp\n");
        #endif
        Operand t1=new_temp();
        Operand t2=new_temp();
        struct InterCodes* code1=translate_Exp(Exp->childlist[0],&ST,t1);
        struct InterCodes* code2=translate_Exp(Exp->childlist[2],&ST,t2);

        if(place!=NULL)
        {
            struct InterCodes* code3=new_code();
            code3->code.kind=IC_MUL;
            code3->code.u.binop.op1=t1;
            code3->code.u.binop.op2=t2;
            code3->code.u.binop.result=place;

            connect_ic(code2,code3);
        }

        connect_ic(code1,code2);
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"DIV")==0)
    {
        #ifdef _DEBUG
            printf("Exp DIV Exp\n");
        #endif
        Operand t1=new_temp();
        Operand t2=new_temp();
        struct InterCodes* code1=translate_Exp(Exp->childlist[0],&ST,t1);
        struct InterCodes* code2=translate_Exp(Exp->childlist[2],&ST,t2);

        if(place!=NULL)
        {
            struct InterCodes* code3=new_code();
            code3->code.kind=IC_DIV;
            code3->code.u.binop.op1=t1;
            code3->code.u.binop.op2=t2;
            code3->code.u.binop.result=place;
            
            connect_ic(code2,code3);
        }
        

        connect_ic(code1,code2);
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"LP")==0)//LP Exp RP
    {
        #ifdef _DEBUG
            printf("LP Exp RP\n");
        #endif
        return translate_Exp(Exp->childlist[1],sym_table,place);
    }
    else if(strcmp(Exp->childlist[0]->name,"MINUS")==0)//MINUS Exp
    {
        #ifdef _DEBUG
            printf("MINUS Exp\n");
        #endif
        Operand t1=new_temp();
        struct InterCodes* code1=translate_Exp(Exp->childlist[1],&ST,t1);

        if(place!=NULL)
        {
            struct InterCodes* code2=new_code();
            code2->code.kind=IC_SUB;
            code2->code.u.binop.op1=new_constant(0);
            code2->code.u.binop.op2=t1;
            code2->code.u.binop.result=place;

            connect_ic(code1,code2);
        }

        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"NOT")==0)//NOT Exp
    {
        #ifdef _DEBUG
            printf("NOT Exp\n");
        #endif
        Operand label1=new_lable();
        Operand label2=new_lable();

        struct InterCodes* code0=new_code();
        code0->code.kind=IC_ASSIGN;
        Operand variable=new_constant(0);
        code0->code.u.assign.right=variable;
        code0->code.u.assign.left=place;

        struct InterCodes* code1=translate_Cond(Exp, label1, label2, &ST);
        
        struct InterCodes* code2_1=new_code();
        code2_1->code.kind=IC_LABLE;
        code2_1->code.u.lable.op_lable=label1;

        struct InterCodes* code2_2=new_code();
        code2_2->code.kind=IC_ASSIGN;
        Operand variable2=new_constant(1);
        code2_2->code.u.assign.right=variable2;
        code2_2->code.u.assign.left=place;

        struct InterCodes* code3=new_code();
        code3->code.kind=IC_LABLE;
        code3->code.u.lable.op_lable=label2;

        connect_ic(code0,code1);
        connect_ic(code1,code2_1);
        connect_ic(code2_1,code2_2);
        connect_ic(code2_2,code3);

        return code0;
    }
    else if(strcmp(Exp->childlist[0]->name,"ID")==0 && Exp->childnum==4)//ID LP Args RP  //Args → Exp COMMA Args | Exp
    {
        #ifdef _DEBUG
            printf("ID LP Args RP\n");
        #endif
        struct Symbol* function = SymbolFind(sym_table,Exp->childlist[0]->extra);
        struct operandlist* arg_list=NULL;
        struct InterCodes* code1=translate_Args(Exp->childlist[2],sym_table,&arg_list);
        if(strcmp(function->name,"write")==0)
        {
            #ifdef _DEBUG
                printf("function name is write\n");
            #endif
            struct InterCodes* code_write=new_code();
            code_write->code.kind=IC_WRITE;
            //printf("%d\n",(int)arg_list);
            code_write->code.u.write.op_write=arg_list->op;
            //printf("halo\n");
            if(place!=NULL)
            {
                struct InterCodes* code_assign=new_code();
                code_assign->code.kind=IC_ASSIGN;
                code_assign->code.u.assign.left=place;
                code_assign->code.u.assign.right=new_constant(0);
                connect_ic(code_write,code_assign);
            }
            
            connect_ic(code1,code_write);
            

            return code1;
        }
        struct InterCodes* code2=NULL;
        while(arg_list!=NULL)
        {
            //printf("HALO\n");
            struct InterCodes* code_arg=new_code();
            code_arg->code.kind=IC_ARG;
            code_arg->code.u.arg.x=arg_list->op;
            if(code2==NULL)
                code2=code_arg;
            else 
                connect_ic(code2,code_arg);
            arg_list=arg_list->next;
        }

        struct InterCodes* code_call=new_code();
        code_call->code.kind=IC_CALL;
        code_call->code.u.call.f=malloc(sizeof(struct Operand_));
        code_call->code.u.call.f->kind=FUNC;
        code_call->code.u.call.f->u.func_name=Exp->childlist[0]->extra;
        code_call->code.u.call.x=place;

        connect_ic(code1,code2);
        connect_ic(code2,code_call);
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"ID")==0 && Exp->childnum==3)//ID LP RP
    {
        #ifdef _DEBUG
            printf("ID LP RP\n");
        #endif
        struct Symbol* function = SymbolFind(sym_table,Exp->childlist[0]->extra);
        if(strcmp(function->name,"read")==0)
        {
            struct InterCodes* code=new_code();
            code->code.kind=IC_READ;
            code->code.u.ret.op_return=place;
            return code;
        }
        struct InterCodes* code=new_code();
        code->code.kind=IC_CALL;
        code->code.u.call.f=malloc(sizeof(struct Operand_));
        code->code.u.call.f->kind=FUNC;
        code->code.u.call.f->u.func_name=Exp->childlist[0]->extra;
        code->code.u.call.x=place;
        
        return code;
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"LB")==0)//Exp LB Exp RB
    {
        #ifdef _DEBUG
            printf("Exp LB Exp RB\n");
        #endif

        // translate_Exp(Exp2, t1);
		// 	t2 = t1*4;
		// 	translate_Exp(Exp1, ID);
		// 	t3 = &ID;
		// 	place = t3+t2;
        
        struct TreeNode* root=Exp;


        int depth=0;
        while(root->childnum!=1 && strcmp(root->childlist[1]->name,"LB")==0)
        {
            root=root->childlist[0];
            depth++;
        }

        int save_depth=depth;
        //printf("aaaaaaaaaaaaaaaaaaaaaaaaaa:  %d\n",depth);
        Type test,test2;
        struct Symbol* s;
        char* name;
        FieldList FL;
        if(root->childnum==1)
        {
            name=root->childlist[0]->extra;
            s=SymbolFind(sym_table,name);
            test=s->type;
        }
        else
        {
            printf("%s\n",root->childlist[2]->extra);
            FL=Find_ID_Struct(sym_table,root->childlist[2]->extra);
            test=FL->type;
        }
        
        test2=test;
        int total_depth=0;
        while(test->kind==ARRAY)
        {
            total_depth++;
            test=test->u.array.elem;
        }
        int flag=0;
        if(test->kind!=BASIC)
        {
            flag=1;
        }
        
        Type cur, next;
        
        if(root->childnum==1)
        {
            cur=s->type;
            next=s->type->u.array.elem;
        }
        else
        {
            cur=test2;
            next=test2->u.array.elem;
        }
        cur->upper=NULL;
        //int ele_size;
        while(depth>1)
        {
            next->upper=cur;
            cur=next;
            next=next->u.array.elem;
            depth--;
        }
        next->upper=cur;
        cur=next;
        //ele_size=szof(next);

        Operand temp_sum=new_temp();
        struct InterCodes* code0=new_code();
        code0->code.kind=IC_ASSIGN;
        code0->code.u.assign.left=temp_sum;
        code0->code.u.assign.right=new_constant(0);

        root=Exp;
        while(root->childnum!=1 && strcmp(root->childlist[1]->name,"LB")==0)
        {
            Operand t=new_temp();
            struct InterCodes* code_exp2=translate_Exp(root->childlist[2],sym_table,t);

            struct InterCodes* code1=new_code();
            code1->code.kind=IC_MUL;
            code1->code.u.binop.result=t;
            code1->code.u.binop.op1=t;
            code1->code.u.binop.op2=new_constant(szof(cur));
            
            struct InterCodes* code2=new_code();
            code2->code.kind=IC_ADD;
            code2->code.u.binop.result=temp_sum;
            code2->code.u.binop.op1=temp_sum;
            code2->code.u.binop.op2=t;


            connect_ic(code0,code_exp2);
            connect_ic(code_exp2,code1);
            connect_ic(code1,code2);

            cur=cur->upper;
            root=root->childlist[0];
        }



        Operand temp1=new_temp();
        // Operand temp2=new_temp();
        struct InterCodes* code_E;
        if(root->childnum==1) temp1->u.var_name=name;
        else
        {
            temp1->kind=ADDRESS;
            code_E=translate_Exp(root,sym_table,temp1);
        }
        // struct InterCodes* code1=new_code();
        // code1->code.kind=IC_ADDR;
        // code1->code.u.addr.left=temp1;
        // code1->code.u.addr.right=temp2;

        
        

        //printf("HHHHHHHHHHHHHHHHHHH  place:%d  depth:%d total_depth:%d\n",(int)place,depth,total_depth);
        if(place!=NULL&&(place->kind==ADDRESS||save_depth<total_depth ||flag==1))
        {
            struct InterCodes* code2=new_code();
            code2->code.kind=IC_ADD;
            code2->code.u.binop.result=place;
            code2->code.u.binop.op1=temp1;
            code2->code.u.binop.op2=temp_sum;
            
            if(root->childnum!=1)
            {
                connect_ic(code0,code_E);
                connect_ic(code_E,code2);
            }
            else
                connect_ic(code0,code2);
            //connect_ic(code1,code2);
            return code0;
        }
        else
        {
            struct InterCodes* code2=new_code();
            Operand temp3=new_temp();
            code2->code.kind=IC_ADD;
            code2->code.u.binop.result=temp3;
            code2->code.u.binop.op1=temp1;
            code2->code.u.binop.op2=temp_sum;
            connect_ic(code0,code2);
            //connect_ic(code1,code2);
            
            if(place!=NULL)
            {
                struct InterCodes* code3 = new_code();
                code3->code.kind=IC_R_DEREF;
                code3->code.u.r_deref.left=place;
                code3->code.u.r_deref.right=temp3;
                connect_ic(code2,code3);
                return code0;
            }
            return NULL;
        }
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"DOT")==0)//Exp DOT ID
    {
        #ifdef _DEBUG
            printf("Exp DOT ID\n");
        #endif
        Operand t1=new_temp();
        t1->kind=ADDRESS;
        struct InterCodes* code1=translate_Exp(Exp->childlist[0],sym_table,t1);

        Operand t2=new_temp();
        struct InterCodes* code2=new_code();
        code2->code.kind=IC_ASSIGN;
        code2->code.u.assign.left=t2;
        code2->code.u.assign.right=new_constant(Find_ID_Struct(sym_table,Exp->childlist[2]->extra)->offset);

        struct InterCodes* code3=new_code();
        code3->code.kind=IC_ADD;
        code3->code.u.binop.result=t1;
        code3->code.u.binop.op1=t1;
        code3->code.u.binop.op2=t2;

        if(place!=NULL)
        {
            if(place->kind==ADDRESS)
            {
                struct InterCodes* code4=new_code();
                code4->code.kind=IC_ASSIGN;
                code4->code.u.assign.left=place;
                code4->code.u.assign.right=t1;
                connect_ic(code3,code4);
            }
            else
            {
                struct InterCodes* code4=new_code();
                code4->code.kind=IC_R_DEREF;
                code4->code.u.assign.left=place;
                code4->code.u.assign.right=t1;
                connect_ic(code3,code4);
            }
        }
        

        connect_ic(code1,code2);
        connect_ic(code2,code3);
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"ID")==0 && Exp->childnum==1)
    {
        #ifdef _DEBUG
            printf("ID\n");
        #endif
        struct Symbol* S = SymbolFind(&ST,Exp->childlist[0]->extra);
        Operand right=malloc(sizeof(struct Operand_));
        right->kind=VARIABLE;
        right->u.var_name=Exp->childlist[0]->extra;

        struct InterCodes* ic=new_code();

        ic->code.kind=IC_ASSIGN;
        ic->code.u.assign.left=place;
        ic->code.u.assign.right=right;
        return ic;
    }
    else if(strcmp(Exp->childlist[0]->name,"INT")==0)
    {
        #ifdef _DEBUG
            printf("INT\n");
        #endif
        Operand right=new_constant(Exp->childlist[0]->INT);

        struct InterCodes* ic=new_code();

        ic->code.kind=IC_ASSIGN;
        ic->code.u.assign.left=place;
        ic->code.u.assign.right=right;
        return ic;
    }
    else if(strcmp(Exp->childlist[0]->name,"FLOAT")==0)//不会出现浮点型常数或者变量
    {
        #ifdef _DEBUG
            printf("FLOAT\n");
        #endif
    }
}

struct InterCodes* translate_Stmt(struct TreeNode* Stmt,struct SymbolTable* sym_table)
// Stmt → Exp SEMI
// | CompSt
// | RETURN Exp SEMI
// | IF LP Exp RP Stmt
// | IF LP Exp RP Stmt ELSE Stmt
// | WHILE LP Exp RP Stmt
{
    #ifdef _DEBUG
        printf("translate_Stmt!");
    #endif
    if(strcmp(Stmt->childlist[0]->name,"Exp")==0)
    {
        #ifdef _DEBUG
            printf(":Stmt → Exp SEMI\n");
        #endif
        return translate_Exp(Stmt->childlist[0],sym_table,NULL);
    }
    else if(strcmp(Stmt->childlist[0]->name,"CompSt")==0)
    {
        #ifdef _DEBUG
            printf(":Stmt → CompSt\n");
        #endif
        return translate_CompSt(Stmt->childlist[0],sym_table);
    }
    else if(strcmp(Stmt->childlist[0]->name,"RETURN")==0)
    {
        #ifdef _DEBUG
            printf(":Stmt → RETURN Exp SEMI\n");
        #endif
        Operand t1=new_temp();
        struct InterCodes* code1=translate_Exp(Stmt->childlist[1],sym_table,t1);
        struct InterCodes* code2=new_code();
        code2->code.kind=IC_RETURN;
        code2->code.u.ret.op_return=t1;
        connect_ic(code1,code2);
        return code1;
    }
    else if(strcmp(Stmt->childlist[0]->name,"IF")==0&&Stmt->childnum==5)
    {
        #ifdef _DEBUG
            printf(":Stmt → IF LP Exp RP Stmt\n");
        #endif
        Operand label1=new_lable();
        Operand label2=new_lable();
        struct InterCodes* code1=translate_Cond(Stmt->childlist[2],label1,label2,sym_table);
        struct InterCodes* code2=translate_Stmt(Stmt->childlist[4],sym_table);
        
        struct InterCodes* code3=new_code();
        code3->code.kind=IC_LABLE;
        code3->code.u.lable.op_lable=label1;
        
        struct InterCodes* code4=new_code();
        code4->code.kind=IC_LABLE;
        code4->code.u.lable.op_lable=label2;

        connect_ic(code1,code3);
        connect_ic(code3,code2);
        connect_ic(code2,code4);

        return code1;
    }
    else if(strcmp(Stmt->childlist[0]->name,"IF")==0&&Stmt->childnum==7)
    {
        #ifdef _DEBUG
            printf(":Stmt → IF LP Exp RP Stmt ELSE Stmt\n");
        #endif
        Operand label1=new_lable();
        Operand label2=new_lable();
        Operand label3=new_lable();

        struct InterCodes* code1=translate_Cond(Stmt->childlist[2],label1,label2,sym_table);
        //printf("code1\n");
        struct InterCodes* code2=translate_Stmt(Stmt->childlist[4],sym_table);
        //printf("code2\n");
        struct InterCodes* code3=translate_Stmt(Stmt->childlist[6],sym_table);
        //printf("code3\n");
        struct InterCodes* code_label1=new_code();
        code_label1->code.kind=IC_LABLE;
        code_label1->code.u.lable.op_lable=label1;
        
        struct InterCodes* code_label2=new_code();
        code_label2->code.kind=IC_LABLE;
        code_label2->code.u.lable.op_lable=label2;

        struct InterCodes* code_label3=new_code();
        code_label3->code.kind=IC_LABLE;
        code_label3->code.u.lable.op_lable=label3;

        struct InterCodes* code_goto=new_code();
        code_goto->code.kind=IC_GOTO;
        code_goto->code.u.gt.op_goto=label3;

        connect_ic(code1,code_label1);
        connect_ic(code_label1,code2);
        connect_ic(code2,code_goto);
        connect_ic(code_goto,code_label2);
        connect_ic(code_label2,code3);
        connect_ic(code3,code_label3);

        return code1;
    }
    else//WHILE LP Exp RP Stmt
    {
        #ifdef _DEBUG
            printf(":Stmt → WHILE LP Exp RP Stmt\n");
        #endif
        Operand label1=new_lable();
        Operand label2=new_lable();
        Operand label3=new_lable();

        struct InterCodes* code1=translate_Cond(Stmt->childlist[2],label2,label3,sym_table);
        struct InterCodes* code2=translate_Stmt(Stmt->childlist[4],sym_table);
        
        struct InterCodes* code_label1=new_code();
        code_label1->code.kind=IC_LABLE;
        code_label1->code.u.lable.op_lable=label1;

        struct InterCodes* code_label2=new_code();
        code_label2->code.kind=IC_LABLE;
        code_label2->code.u.lable.op_lable=label2;

        struct InterCodes* code_label3=new_code();
        code_label3->code.kind=IC_LABLE;
        code_label3->code.u.lable.op_lable=label3;

        struct InterCodes* code_goto=new_code();
        code_goto->code.kind=IC_GOTO;
        code_goto->code.u.gt.op_goto=label1;

        connect_ic(code_label1,code1);
        connect_ic(code1,code_label2);
        connect_ic(code_label2,code2);
        connect_ic(code2,code_goto);
        connect_ic(code_goto,code_label3);

        return code_label1;
    }
}

struct InterCodes*  translate_CompSt(struct TreeNode* CompSt,struct SymbolTable* sym_table)
{
    #ifdef _DEBUG
        printf("translate_CompSt!\n");
    #endif
    //CompSt → LC DefList StmtList RC
    struct InterCodes* code1=translate_DefList(CompSt->childlist[1]);
    struct InterCodes* code2=translate_StmtList(CompSt->childlist[2],sym_table);
    if(code1!=NULL) connect_ic(code1,code2);
    else code1=code2;
    return code1;
}

struct InterCodes* translate_StmtList(struct TreeNode* StmtList,struct SymbolTable* sym_table)
{
    #ifdef _DEBUG
        printf("translate_StmtList!\n");
    #endif
    //StmtList → Stmt StmtList | e
    if(StmtList->childnum==2)
    {
        struct InterCodes* code1=translate_Stmt(StmtList->childlist[0],sym_table);
        
        struct InterCodes* code2=translate_StmtList(StmtList->childlist[1],sym_table);
        #ifdef _DEBUG
            printf("Connect Stmt StmtList\n");
        #endif
        connect_ic(code1,code2);
        return code1;
    }
    else
        return NULL;
}


struct InterCodes* translate_Cond(struct TreeNode* Exp,Operand label_true, Operand label_false,struct SymbolTable* sym_table)
{
    #ifdef _DEBUG
        printf("translate_Cond!\n");
    #endif
    if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"RELOP")==0)
    {
        Operand t1=new_temp();
        Operand t2=new_temp();
        struct InterCodes* code1=translate_Exp(Exp->childlist[0],sym_table,t1);
        struct InterCodes* code2=translate_Exp(Exp->childlist[2],sym_table,t2);
        Operand op=get_relop(Exp->childlist[1]);

        struct InterCodes* code3=new_code();
        code3->code.kind=IC_IFGOTO;
        code3->code.u.ifgt.x=t1;
        code3->code.u.ifgt.relop=op;
        code3->code.u.ifgt.y=t2;
        code3->code.u.ifgt.z=label_true;

        struct InterCodes* code_goto=new_code();
        code_goto->code.kind=IC_GOTO;
        code_goto->code.u.gt.op_goto=label_false;

        connect_ic(code1,code2);
        connect_ic(code2,code3);
        connect_ic(code3,code_goto);
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"NOT")==0)//NOT Exp
    {
        return translate_Cond(Exp->childlist[1],label_false,label_true,sym_table);
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"AND")==0)
    {
        Operand label1=new_lable();
        struct InterCodes* code1=translate_Cond(Exp->childlist[0],label1,label_false,sym_table);
        struct InterCodes* code2=translate_Cond(Exp->childlist[2],label_true,label_false,sym_table);

        struct InterCodes* code_label=new_code();
        code_label->code.kind=IC_LABLE;
        code_label->code.u.lable.op_lable=label1;

        connect_ic(code1,code_label);
        connect_ic(code_label,code2);
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"OR")==0)
    {
        Operand label1=new_lable();
        struct InterCodes* code1=translate_Cond(Exp->childlist[0],label_true,label1,sym_table);
        struct InterCodes* code2=translate_Cond(Exp->childlist[2],label_true,label_false,sym_table);

        struct InterCodes* code_label=new_code();
        code_label->code.kind=IC_LABLE;
        code_label->code.u.lable.op_lable=label1;

        connect_ic(code1,code_label);
        connect_ic(code_label,code2);
        return code1;
    }
    else
    {
        Operand t1=new_temp();
        struct InterCodes* code1=translate_Exp(Exp,sym_table,t1);
        struct InterCodes* code2=new_code();
        code2->code.kind=IC_IFGOTO;
        code2->code.u.ifgt.x=t1;

        code2->code.u.ifgt.relop=malloc(sizeof(struct Operand_));
        code2->code.u.ifgt.relop->kind=REL;
        code2->code.u.ifgt.relop->u.relop=malloc(5);
        strcpy(code2->code.u.ifgt.relop->u.relop,"!=");

        code2->code.u.ifgt.y=new_constant(0);
        code2->code.u.ifgt.z=label_true;

        struct InterCodes* code_goto=new_code();
        code_goto->code.kind=IC_GOTO;
        code_goto->code.u.gt.op_goto=label_false;

        connect_ic(code1,code2);
        connect_ic(code2,code_goto);

        return code1;
    }
}

struct InterCodes* translate_Args(struct TreeNode* Args,struct SymbolTable* sym_table,struct operandlist** arg_list)
{
    #ifdef _DEBUG
        printf("translate_Args!\n");
    #endif
    if(Args->childnum==1)//Exp
    {
        Operand t1=new_temp();
        //if(strcmp(Args->childlist[0]->childlist[0]->name,"Exp")==0 && strcmp(Args->childlist[0]->childlist[1]->name,"LB")==0)
        //    t1->kind=ADDRESS;
        struct InterCodes* code1=translate_Exp(Args->childlist[0],sym_table,t1);
        struct operandlist* temp=malloc(sizeof(struct operandlist));
        temp->op=t1;
        temp->next=*arg_list;
        temp->prev=NULL;
        if(*arg_list!=NULL) (*arg_list)->prev=temp;
        *arg_list=temp;
        return code1;
    }
    else//Exp COMMA Args
    {
        Operand t1=new_temp();
        struct InterCodes* code1=translate_Exp(Args->childlist[0],sym_table,t1);
        struct operandlist* temp=malloc(sizeof(struct operandlist));
        temp->op=t1;
        temp->next=*arg_list;
        temp->prev=NULL;
        if(*arg_list!=NULL) (*arg_list)->prev=temp;
        *arg_list=temp;
        
        struct InterCodes* code2=translate_Args(Args->childlist[2],sym_table,arg_list);
        connect_ic(code1,code2);
        return code1;
    }
}

void generate_ic(struct TreeNode* root)
{
    IC_head=translate_program(root);
    
}

struct InterCodes* translate_program(struct TreeNode* Program)
{
    //Program → ExtDefList
    #ifdef _DEBUG
        printf("translate_program!\n");
    #endif
    return translate_ExtDefList(Program->childlist[0]);
}

struct InterCodes* translate_ExtDefList(struct TreeNode* ExtDefList)
{
    //ExtDefList → ExtDef ExtDefList | e
    #ifdef _DEBUG
        printf("translate_ExtDefList!\n");
    #endif
    if(ExtDefList->childnum>0)
    {
        struct InterCodes* code1=translate_ExtDef(ExtDefList->childlist[0]);
        
        struct InterCodes* code2=translate_ExtDefList(ExtDefList->childlist[1]);
        if(code1!=NULL) connect_ic(code1,code2);
        else code1=code2;
        return code1;
    }
    else
    {
        return NULL;
    }
}

struct InterCodes* translate_ExtDef(struct TreeNode* ExtDef)
{
    // ExtDef → Specifier ExtDecList SEMI  没有全局变量的使用
    // | Specifier SEMI
    // | Specifier FunDec CompSt
    #ifdef _DEBUG
        printf("translate_ExtDef!\n");
    #endif
    if(strcmp(ExtDef->childlist[1]->name,"ExtDecList")==0)//ExtDef → Specifier ExtDecList SEMI
    {
        //printf("ExtDef → Specifier ExtDecList SEMI\n");
        return NULL;
    }
    else if(strcmp(ExtDef->childlist[1]->name,"SEMI")==0)//Specifier SEMI
    {
        //printf("ExtDef → Specifier SEMI\n");
        return NULL;
    }
    else if(strcmp(ExtDef->childlist[2]->name,"CompSt")==0)//Specifier FunDec CompSt
    {
        //不会返回结构体类型的值
        struct InterCodes* code1 = translate_FunDec(ExtDef->childlist[1]);
        struct InterCodes* code2 = translate_CompSt(ExtDef->childlist[2],&ST);
        connect_ic(code1,code2);
        return code1;
    }
    else//Specifier FunDec SEMI  函数只会进行一次定义（没有函数声明）。
    {

    }
}

struct InterCodes* translate_FunDec(struct TreeNode* FunDec)
{
    //FunDec → ID LP VarList RP
    //       | ID LP RP
    #ifdef _DEBUG
        printf("translate_FunDec!\n");
    #endif
    if(FunDec->childnum==4)
    {
        struct InterCodes* code1=new_code();
        code1->code.kind=IC_FUNC;
        code1->code.u.func.op_func=malloc(sizeof(struct Operand_));
        code1->code.u.func.op_func->kind=FUNC;
        code1->code.u.func.op_func->u.func_name=FunDec->childlist[0]->extra;

        struct InterCodes* code2=translate_VarList(FunDec->childlist[2]);

        connect_ic(code1,code2);
        return code1;
    }
    else
    {
        struct InterCodes* code1=new_code();
        code1->code.kind=IC_FUNC;
        code1->code.u.func.op_func=malloc(sizeof(struct Operand_));
        code1->code.u.func.op_func->kind=FUNC;
        code1->code.u.func.op_func->u.func_name=FunDec->childlist[0]->extra;
        return code1;
    }
}

struct InterCodes* translate_VarList(struct TreeNode* VarList)
{
    //VarList → ParamDec COMMA VarList
    //        | ParamDec
    #ifdef _DEBUG
        printf("translate_VarList!\n");
    #endif
    if(VarList->childnum>1)//VarList → ParamDec COMMA VarList 
    {
        struct InterCodes* code1=translate_ParamDec(VarList->childlist[0]);
        struct InterCodes* code2=translate_VarList(VarList->childlist[2]);
        connect_ic(code1,code2);
        return code1;
    }
    else
    {
        struct InterCodes* code1=translate_ParamDec(VarList->childlist[0]);
        return code1;
    }
}

struct InterCodes* translate_ParamDec(struct TreeNode* ParamDec)
{
    //ParamDec → Specifier VarDec
    //Type t=specifier(ParamDec->childlist[0]);
    #ifdef _DEBUG
        printf("translate_ParamDec!\n");
    #endif
    struct TreeNode* VarDec=ParamDec->childlist[1];//VarDec → ID | VarDec LB INT RB
    while(VarDec->childnum>1)
    {
        VarDec=VarDec->childlist[0];
    }
    char* name=VarDec->childlist[0]->extra;

    struct InterCodes* code=new_code();
    code->code.kind=IC_PARAM;
    code->code.u.param.x=malloc(sizeof(struct Operand_));
    code->code.u.param.x->kind=VARIABLE;
    code->code.u.param.x->u.var_name=name;

    return code;
}

struct InterCodes* translate_DefList(struct TreeNode* DefList)
{
    //DefList → Def DefList | e
    #ifdef _DEBUG
        printf("translate_DefList!\n");
    #endif
    if(DefList->childnum>1)
    {
        struct InterCodes* code1=translate_Def(DefList->childlist[0]);
        struct InterCodes* code2=translate_DefList(DefList->childlist[1]);
        if(code1!=NULL) connect_ic(code1,code2);
        else code1=code2;
        return code1;
    }
    else
    {
        return NULL;
    }
}

struct InterCodes* translate_Def(struct TreeNode* Def)
{
    #ifdef _DEBUG
        printf("translate_Def!\n");
    #endif
    //Def → Specifier DecList SEMI
    return translate_DecList(Def->childlist[1]);
}

struct InterCodes* translate_DecList(struct TreeNode* DecList)
{
    //DecList → Dec | Dec COMMA DecList
    #ifdef _DEBUG
        printf("translate_DecList!\n");
    #endif
    if(DecList->childnum==1)
    {
        struct InterCodes* code1=translate_Dec(DecList->childlist[0]);
        return code1;
    }
    else
    {
        struct InterCodes* code1=translate_Dec(DecList->childlist[0]);
        struct InterCodes* code2=translate_DecList(DecList->childlist[2]);
        if(code1!=NULL) connect_ic(code1,code2);
        else code1=code2;
        return code1;
    }
}

struct InterCodes* translate_Dec(struct TreeNode* Dec)
{
    //Dec → VarDec | VarDec ASSIGNOP Exp
    #ifdef _DEBUG
        printf("translate_Dec!\n");
    #endif
    if(Dec->childnum==1)
    {
        struct InterCodes* code1=translate_VarDec(Dec->childlist[0],NULL);
        return code1;
    }
    else
    {
        Operand t1=new_temp();
        struct InterCodes* code1=translate_VarDec(Dec->childlist[0],t1);
        Operand t2=new_temp();
        struct InterCodes* code2=translate_Exp(Dec->childlist[2],&ST,t2);

        struct InterCodes* code3=new_code();
        code3->code.kind=IC_ASSIGN;
        code3->code.u.assign.left=t1;
        code3->code.u.assign.right=t2;

        if(code1==NULL) code1=code2;
        else connect_ic(code1,code2);

        connect_ic(code2,code3);

        return code1;
    }
}

struct InterCodes* translate_VarDec(struct TreeNode* VarDec,Operand place)
{
    //VarDec → ID | VarDec LB INT RB
    #ifdef _DEBUG
        printf("translate_VarDec!\n");
    #endif

    if(place==NULL)
    {
        if(VarDec->childnum==1)
        {
            char* id=VarDec->childlist[0]->extra;
            struct Symbol* s = SymbolFind(&ST,id);
            if(s->type->kind==BASIC)
                return NULL;
            else if(s->type->kind==STRUCTURE)
            {
                char* id=VarDec->childlist[0]->extra;
                struct Symbol* s = SymbolFind(&ST,id);

                struct InterCodes* code=new_code();
                code->code.kind=IC_DEC;
                code->code.u.dec.x=new_temp();
                code->code.u.dec.size=szof(s->type);

                Operand temp2=new_temp();
                temp2->u.var_name=id;
                struct InterCodes* code1=new_code();
                code1->code.kind=IC_ADDR;
                code1->code.u.addr.left=temp2;
                code1->code.u.addr.right=code->code.u.dec.x;

                connect_ic(code,code1);
                return code;
            }
            else
            {
                printf("ERROR: type error in translate_VarDec\n");
                return NULL;
            }

        }
        else
        {
            while(VarDec->childnum!=1)
            {
                VarDec=VarDec->childlist[0];
            }
            char* id=VarDec->childlist[0]->extra;
            struct Symbol* s = SymbolFind(&ST,id);
            
            struct InterCodes* code=new_code();
            code->code.kind=IC_DEC;
            Operand t1=new_temp();
            code->code.u.dec.x=t1;
            code->code.u.dec.size=szof(s->type);

            Operand t2=new_temp();
            t2->u.var_name=id;
            struct InterCodes* code1=new_code();
            code1->code.kind=IC_ADDR;
            code1->code.u.addr.left=t2;
            code1->code.u.addr.right=t1;

            connect_ic(code,code1);
            return code;
        }
    }
    else
    {
        if(VarDec->childnum==1)
        {
            char* id=VarDec->childlist[0]->extra;
            
            struct Symbol* s = SymbolFind(&ST,id);
            if(s->type->kind==BASIC)
            {
                place->u.var_name=id;
                //printf("%s\n",place->u.var_name);
                return NULL;
            }    
            else if(s->type->kind==ARRAY || s->type->kind==STRUCTURE)
            {
                printf("ERROR: type error in translate_VarDec\n");
                return NULL;
            }
        }
        else
        {
            printf("ERROR: type error in translate_VarDec\n");
            return NULL;
        }
    }
}

int szof(Type t)
{
    if(t->kind==BASIC)
    {
        return 4;
    }
    else if(t->kind==ARRAY)
    {
        return t->u.array.size*szof(t->u.array.elem);
    }
    else if(t->kind==STRUCTURE)
    {
        FieldList FL=t->u.structure;
        int size=0;
        while(FL!=NULL)
        {
            size+=szof(FL->type);
            FL=FL->tail;
        }
        return size;
    }
    else
    {
        printf("error szof type\n");
        return 0;
    }
}