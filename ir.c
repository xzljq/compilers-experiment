#include "ir.h"

//extern struct TreeNode* root;
extern struct SymbolTable ST;

struct InterCodes* IC_head = NULL;
int temp_count=0;
int lable_count=0;

void writeToFile(struct InterCodes* head,FILE* f)
{
    while(head!=NULL)
    {
        //printf("%d\n",head->code.kind);
        if(head->code.kind==IC_ASSIGN)
        {
            if(head->code.u.assign.left==NULL)
            {
                return;
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
            fprintf(f, " [%d]",head->code.u.dec.size);
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
    temp->u.var_name=malloc(20);
    sprintf(temp->u.var_name,"t%d", temp_count++);
    return temp;
}

Operand new_lable()
{
    Operand lable=malloc(sizeof(struct Operand_));
    lable->kind=LABLE;
    lable->u.lable_name=malloc(20);
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
    //sprintf("connect %d %d\n",(int)code1,(int)code2);
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

        }
        else//结构体特定域的访问
        {

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
        Operand variable=malloc(sizeof(struct Operand_));
        variable->kind=CONSTANT;
        variable->u.value=0;
        code0->code.u.assign.right=variable;
        code0->code.u.assign.left=place;

        struct InterCodes* code1=translate_Cond(Exp, label1, label2, &ST);
        
        struct InterCodes* code2_1=new_code();
        code2_1->code.kind=IC_LABLE;
        code2_1->code.u.lable.op_lable=label1;

        struct InterCodes* code2_2=new_code();
        code2_2->code.kind=IC_ASSIGN;
        Operand variable2=malloc(sizeof(struct Operand_));
        variable2->kind=CONSTANT;
        variable2->u.value=1;
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
        Operand variable=malloc(sizeof(struct Operand_));
        variable->kind=CONSTANT;
        variable->u.value=0;
        code0->code.u.assign.right=variable;
        code0->code.u.assign.left=place;

        struct InterCodes* code1=translate_Cond(Exp, label1, label2, &ST);
        
        struct InterCodes* code2_1=new_code();
        code2_1->code.kind=IC_LABLE;
        code2_1->code.u.lable.op_lable=label1;

        struct InterCodes* code2_2=new_code();
        code2_2->code.kind=IC_ASSIGN;
        Operand variable2=malloc(sizeof(struct Operand_));
        variable2->kind=CONSTANT;
        variable2->u.value=1;
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
        Operand variable=malloc(sizeof(struct Operand_));
        variable->kind=CONSTANT;
        variable->u.value=0;
        code0->code.u.assign.right=variable;
        code0->code.u.assign.left=place;

        struct InterCodes* code1=translate_Cond(Exp, label1, label2, &ST);
        
        struct InterCodes* code2_1=new_code();
        code2_1->code.kind=IC_LABLE;
        code2_1->code.u.lable.op_lable=label1;

        struct InterCodes* code2_2=new_code();
        code2_2->code.kind=IC_ASSIGN;
        Operand variable2=malloc(sizeof(struct Operand_));
        variable2->kind=CONSTANT;
        variable2->u.value=1;
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
        struct InterCodes* code3=new_code();
        code3->code.kind=IC_ADD;
        code3->code.u.binop.op1=t1;
        code3->code.u.binop.op2=t2;
        code3->code.u.binop.result=place;

        connect_ic(code1,code2);
        connect_ic(code2,code3);
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
        struct InterCodes* code3=new_code();
        code3->code.kind=IC_SUB;
        code3->code.u.binop.op1=t1;
        code3->code.u.binop.op2=t2;
        code3->code.u.binop.result=place;

        connect_ic(code1,code2);
        connect_ic(code2,code3);
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
        struct InterCodes* code3=new_code();
        code3->code.kind=IC_MUL;
        code3->code.u.binop.op1=t1;
        code3->code.u.binop.op2=t2;
        code3->code.u.binop.result=place;

        connect_ic(code1,code2);
        connect_ic(code2,code3);
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
        struct InterCodes* code3=new_code();
        code3->code.kind=IC_DIV;
        code3->code.u.binop.op1=t1;
        code3->code.u.binop.op2=t2;
        code3->code.u.binop.result=place;

        connect_ic(code1,code2);
        connect_ic(code2,code3);
        return code1;
    }
    else if(strcmp(Exp->childlist[0]->name,"LP")==0)//LP Exp RP
    {
        #ifdef _DEBUG
            printf("LP Exp RP\n");
        #endif
    }
    else if(strcmp(Exp->childlist[0]->name,"MINUS")==0)//MINUS Exp
    {
        #ifdef _DEBUG
            printf("MINUS Exp\n");
        #endif
        Operand t1=new_temp();
        struct InterCodes* code1=translate_Exp(Exp->childlist[1],&ST,t1);
        struct InterCodes* code2=new_code();
        code2->code.kind=IC_SUB;
        code2->code.u.binop.op1=malloc(sizeof(struct Operand_));
        code2->code.u.binop.op1->kind=CONSTANT;
        code2->code.u.binop.op1->u.value=0;
        code2->code.u.binop.op2=t1;
        code2->code.u.binop.result=place;

        connect_ic(code1,code2);
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
        Operand variable=malloc(sizeof(struct Operand_));
        variable->kind=CONSTANT;
        variable->u.value=0;
        code0->code.u.assign.right=variable;
        code0->code.u.assign.left=place;

        struct InterCodes* code1=translate_Cond(Exp, label1, label2, &ST);
        
        struct InterCodes* code2_1=new_code();
        code2_1->code.kind=IC_LABLE;
        code2_1->code.u.lable.op_lable=label1;

        struct InterCodes* code2_2=new_code();
        code2_2->code.kind=IC_ASSIGN;
        Operand variable2=malloc(sizeof(struct Operand_));
        variable2->kind=CONSTANT;
        variable2->u.value=1;
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
            struct InterCodes* code_arg=new_code();
            code_arg->code.kind=IC_ARG;
            code_arg->code.u.arg.x=arg_list->op;
            connect_ic(code2,code_arg);
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
    }
    else if(strcmp(Exp->childlist[0]->name,"Exp")==0 && strcmp(Exp->childlist[1]->name,"DOT")==0)//Exp DOT ID
    {
        #ifdef _DEBUG
            printf("Exp DOT ID\n");
        #endif
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
        Operand right=malloc(sizeof(struct Operand_));
        right->kind=CONSTANT;
        right->u.value=Exp->childlist[0]->INT;

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
    //struct InterCodes* code1=translate_DefList(CompSt->childlist[1]);
    struct InterCodes* code2=translate_StmtList(CompSt->childlist[2],sym_table);
    //connect_ic(code1,code2);
    return code2;
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

        code2->code.u.ifgt.y=malloc(sizeof(struct Operand_));
        code2->code.u.ifgt.y->kind=CONSTANT;
        code2->code.u.ifgt.y->u.value=0;
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
        (*arg_list)->prev=temp;
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
        connect_ic(code1,code2);
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
    }
    else if(strcmp(ExtDef->childlist[1]->name,"SEMI")==0)//Specifier SEMI
    {
        //printf("ExtDef → Specifier SEMI\n");
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