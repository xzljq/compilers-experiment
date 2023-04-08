#include "mips32.h"
#define LOOK_AHEAD 5
//extern struct InterCodes* IC_head;
extern FILE* fp2;
extern struct SymbolTable ST;

struct VarDesc* VarDescList;//have a useless head

char* reg_name[] = 
{
	"$0",  
    "$at", 
    "$v0", "$v1", 
    "$a0", "$a1", "$a2", "$a3", 
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", 
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", 
    "$t8", "$t9", 
    "$k0", "$k1", 
    "$gp", 
    "$sp", 
    "$fp", //$30 帧指针
    "$ra" //$31 返回地址
};

struct RegDesc RegList[32];//1-busy 0-idle


void init_asm(struct InterCodes* head)
{
    fprintf(fp2, ".data\n");
	fprintf(fp2, "_prompt: .asciiz \"Enter an integer:\"\n");
	fprintf(fp2, "_ret: .asciiz \"\\n\"\n");
	fprintf(fp2, ".globl main\n");
    //数据段可以为汇编代码中所要用到的常量和全局变量申请空间
	
	fprintf(fp2, ".text\n");
	fprintf(fp2, "read:\n");
	fprintf(fp2, "  li $v0, 4\n");
	fprintf(fp2, "  la $a0, _prompt\n");
	fprintf(fp2, "  syscall\n");
	fprintf(fp2, "  li $v0, 5\n");
	fprintf(fp2, "  syscall\n");
	fprintf(fp2, "  jr $ra\n");
    fprintf(fp2, "\n");

	fprintf(fp2, "write:\n");
	fprintf(fp2, "  li $v0, 1\n");
	fprintf(fp2, "  syscall\n");
	fprintf(fp2, "  li $v0, 4\n");
	fprintf(fp2, "  la $a0, _ret\n");
	fprintf(fp2, "  syscall\n");
	fprintf(fp2, "  move $v0, $0\n");
	fprintf(fp2, "  jr $ra\n");
}

void gen_mips(struct InterCodes* head)
{
    //RegList[0].op=new_constant(0);
    init_asm(head);
    setBasicBlock(head);
    VarDescList=malloc(sizeof(struct VarDesc));
    while(head!=NULL)
    {
        head = process_block(head);
    }
}

void printReg()
{
    printf("----------------\n");
    for(int i=8;i<26;i++)
    {
        if(RegList[i].use==1)
            printf("No.%d Use:%d Name:%s\n",i,1,Opname(RegList[i].op));
        else
            printf("No.%d Use:%d\n",i,0);
    }
    printf("----------------\n");
}

int Ensure(Operand x,struct InterCodes* head)
{
    int result=-1;
    int r=-1;
    for(int i=8;i<26;i++)
    {
        // fprintf(fp2,"%s ",x->u.var_name);
        // if(RegList[i].op!=NULL) fprintf(fp2,"%s cmp\n",RegList[i].op->u.var_name);
        // else fprintf(fp2,"\n");
        if(Opcmp(RegList[i].op,x)==0)
        {
            r=i;
            break;
        }
    }
    

    if(r!=-1 && RegList[r].use==1)//x is already in register r
    {
        result=r;
    }
    else
    {
        result=Allocate(x,head);
        //printf("hello\n");
        
        if(x->kind==VARIABLE || x->kind==ADDRESS) fprintf(fp2,"  lw %s, %d($fp)\n",reg_name[result],mem_offset(x));
        else if(x->kind==CONSTANT) fprintf(fp2,"  li %s, %s\n",reg_name[result],Opname(x));
    }
    assert(result>0);
    RegList[result].use=1;
    RegList[result].op=x;
    return result;
}

int Allocate(Operand x,struct InterCodes* head)
{
    int result=-1;
    int r=-1;
    for(int i=8;i<26;i++)
    {
        if(RegList[i].use==0)
        {
            r=i;
            break;
        }
    }
    if(r!=-1)//there exists a register r that currently has not been assigned to any variable
    {
        result=r;
    }
    else
    {
        flush_reg(head);
        for(int i=8;i<26;i++)
        {
            if(RegList[i].use==0)
            {
                r=i;
                break;
            }
        }
        result=r;
    }
    RegList[result].use=1;
    RegList[result].op=x;
    printReg();
    return result;
}

void flush_reg(struct InterCodes* head)
{
    int count=0;
    int block_no=head->code.BasicBlock;
    while(head!=NULL && head->code.BasicBlock==block_no && count<LOOK_AHEAD)
    {
        OpList ops=get_oplist(head);
        while(ops!=NULL)
        {
            for(int i=8;i<26;i++)
            {
                if(RegList[i].use>0 && Opcmp(RegList[i].op,ops->op)==0)
                {
                    if(RegList[i].use==1)
                    {
                        RegList[i].use=2;
                    }
                }
            }
            ops=ops->next;
        }
        head=head->next;
        count++;
    }

    for(int i=8;i<26;i++)
    {
        RegList[i].use--;
    }
    for(int i=8;i<26;i++)
    {
        if(RegList[i].use==0 && (RegList[i].op->kind==VARIABLE || RegList[i].op->kind==ADDRESS))
        {
            fprintf(fp2,"  sw %s, %d($fp)\n",reg_name[i],mem_offset(RegList[i].op));
        }
    }
}

OpList get_oplist(struct InterCodes* head)
{
    OpList oplist_head=malloc(sizeof(struct OpList_));
    oplist_head->next=NULL;
    if(head->code.kind==IC_ASSIGN)
    {
        oplist_head->op=head->code.u.assign.right;
        oplist_head->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next=NULL;
        oplist_head->next->op=head->code.u.assign.left;
        return oplist_head;
    }
    else if(head->code.kind==IC_ADD || head->code.kind==IC_SUB || head->code.kind==IC_MUL || head->code.kind==IC_DIV)
    {
        oplist_head->op=head->code.u.binop.result;
        oplist_head->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next=NULL;
        oplist_head->next->op=head->code.u.binop.op1;
        oplist_head->next->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next->next=NULL;
        oplist_head->next->next->op=head->code.u.binop.op2;
        return oplist_head;
    }
    else if(head->code.kind==IC_LABLE)
    {
        oplist_head->op=head->code.u.lable.op_lable;
        return oplist_head;
    }
    else if(head->code.kind==IC_FUNC)
    {
        oplist_head->op=head->code.u.func.op_func;
        return oplist_head;
    }
    else if(head->code.kind==IC_RETURN)
    {
        oplist_head->op=head->code.u.ret.op_return;
        return oplist_head;
    }
    else if(head->code.kind==IC_GOTO)
    {
        oplist_head->op=head->code.u.gt.op_goto;
        return oplist_head;
    }
    else if(head->code.kind==IC_IFGOTO)
    {
        oplist_head->op=head->code.u.ifgt.x;
        oplist_head->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next=NULL;
        oplist_head->next->op=head->code.u.ifgt.y;
        oplist_head->next->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next->next=NULL;
        oplist_head->next->next->op=head->code.u.ifgt.z;
        return oplist_head;
    }
    else if(head->code.kind==IC_READ)
    {
        oplist_head->op=head->code.u.read.op_read;
        return oplist_head;
    }
    else if(head->code.kind==IC_WRITE)
    {
        oplist_head->op=head->code.u.write.op_write;
        return oplist_head;
    }
    else if(head->code.kind==IC_CALL)
    {
        oplist_head->op=head->code.u.call.x;
        oplist_head->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next=NULL;
        oplist_head->next->op=head->code.u.call.f;
        return oplist_head;
    }
    else if(head->code.kind==IC_ARG)
    {
        oplist_head->op=head->code.u.arg.x;
        return oplist_head;
    }
    else if(head->code.kind==IC_DEC)
    {
        oplist_head->op=head->code.u.dec.x;
        return oplist_head;
    }
    else if(head->code.kind==IC_PARAM)
    {
       oplist_head->op=head->code.u.param.x;
        return oplist_head;
    }
    else if(head->code.kind==IC_ADDR)
    {
        oplist_head->op=head->code.u.addr.right;
        oplist_head->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next=NULL;
        oplist_head->next->op=head->code.u.addr.left;
        return oplist_head;
    }
    else if(head->code.kind==IC_R_DEREF)
    {
        oplist_head->op=head->code.u.r_deref.right;
        oplist_head->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next=NULL;
        oplist_head->next->op=head->code.u.r_deref.left;
        return oplist_head;
    }
    else if(head->code.kind==IC_L_DEREF)
    {
        oplist_head->op=head->code.u.l_deref.right;
        oplist_head->next=malloc(sizeof(struct OpList_));
        oplist_head->next->next=NULL;
        oplist_head->next->op=head->code.u.l_deref.left;
        return oplist_head;
    }
    else
    {
        printf("???why you get here?\n");
        exit(2);
    }
}

char* Opname(Operand op)
{
    if(op->kind==VARIABLE)
    {
        return op->u.var_name;
    }
    else if(op->kind==CONSTANT)
    {
        char* res=malloc(20);
        sprintf(res,"%d",op->u.value);
        return res;
    }
    else if(op->kind==ADDRESS)
    {
        return op->u.var_name;
    }
    else
    {
        printf("Opname error ,opkind:%d\n",op->kind);
        exit(1);
        return NULL;
    }
}

//ret:1-unequal 0-equal
int Opcmp(Operand op1,Operand op2)
{
    if(op1==NULL||op2==NULL)
        return 1;
    if(op1->kind!=op2->kind)
        return 1;
    if(op1->kind==VARIABLE)
    {
        if(strcmp(op1->u.var_name,op2->u.var_name)==0)
        {
            return 0;
        }
        else   return 1;
    }
    else if(op1->kind==CONSTANT)
    {
        if(op1->u.value==op2->u.value)
        {
            return 0;
        }
        else   return 1;
    }
    else if(op1->kind==ADDRESS)
    {
        if(strcmp(op1->u.var_name,op2->u.var_name)==0)
        {
            return 0;
        }
        else   return 1;
    }
    else
    {
        printf("operand cmp error\n");
        exit(1);
        return 1;
    }

}

struct InterCodes* process_block(struct InterCodes* head)//局部寄存器分配算法
{
    struct InterCodes* block_head=head , *block_tail;
    while(head->next!=NULL && head->next->code.BasicBlock==head->code.BasicBlock)
    {
        head=head->next;
    }
    block_tail=head;
    head=block_head;

    //TODO:save s0-s7

    RefreshRegList();
    while(head!=NULL && head->prev!=block_tail)
    {
        if(head->code.kind==IC_ASSIGN)
        {
            //fprintf(fp2,"IC_ASSIGN\n");
            if(head->code.u.assign.right->kind==CONSTANT)
            {
                int r_x=Ensure(head->code.u.assign.left,head);
                fprintf(fp2, "  li %s, %d\n", reg_name[r_x], head->code.u.assign.right->u.value);
            }
            else
            {
                int r_x=Ensure(head->code.u.assign.left,head);
                int r_y=Ensure(head->code.u.assign.right,head);

                fprintf(fp2, "  move %s, %s\n", reg_name[r_x], reg_name[r_y]);  
            }
            
        }
        else if(head->code.kind==IC_ADD)
        {
            int r_x=Ensure(head->code.u.binop.op1,head);
            int r_y=Ensure(head->code.u.binop.op2,head);
            int r_z=Ensure(head->code.u.binop.result,head);

            fprintf(fp2, "  add %s, %s, %s\n",reg_name[r_z],reg_name[r_x],reg_name[r_y]);
        }
        else if(head->code.kind==IC_SUB)
        {
            int r_x=Ensure(head->code.u.binop.op1,head);
            int r_y=Ensure(head->code.u.binop.op2,head);
            int r_z=Ensure(head->code.u.binop.result,head);

            fprintf(fp2, "  sub %s, %s, %s\n",reg_name[r_z],reg_name[r_x],reg_name[r_y]);
        }
        else if(head->code.kind==IC_MUL)
        {
            int r_x=Ensure(head->code.u.binop.op1,head);
            int r_y=Ensure(head->code.u.binop.op2,head);
            int r_z=Ensure(head->code.u.binop.result,head);

            fprintf(fp2, "  mul %s, %s, %s\n",reg_name[r_z],reg_name[r_x],reg_name[r_y]);
        }
        else if(head->code.kind==IC_DIV)
        {
            int r_x=Ensure(head->code.u.binop.op1,head);
            int r_y=Ensure(head->code.u.binop.op2,head);
            int r_z=Ensure(head->code.u.binop.result,head);

            fprintf(fp2, "  div %s, %s\n",reg_name[r_x],reg_name[r_y]);
            fprintf(fp2, "  mflo %s\n", reg_name[r_z]);	
        }
        else if(head->code.kind==IC_LABLE)
        {
            fprintf(fp2, "%s:\n",head->code.u.lable.op_lable->u.lable_name);
        }
        else if(head->code.kind==IC_FUNC)
        {
            int offset=Init_func(head);//frame size

            fprintf(fp2, "\n");
            fprintf(fp2, "%s:\n",head->code.u.func.op_func->u.func_name);
            fprintf(fp2, "  addi $sp, $sp, -4\n");
            fprintf(fp2, "  sw $fp, 0($sp)\n");
            fprintf(fp2, "  move $fp, $sp\n");
            fprintf(fp2, "  addi $sp, $sp, %d\n",offset+4);
        }
        else if(head->code.kind==IC_RETURN)
        {
			int r_x = Ensure(head->code.u.ret.op_return,head);
			fprintf(fp2, "  move $v0, %s\n", reg_name[r_x]);
            fprintf(fp2, "  move $sp, $fp\n");
            fprintf(fp2, "  lw $fp, 0($sp)\n");
            fprintf(fp2, "  addi $sp, $sp, 4\n");
            fprintf(fp2, "  jr $ra\n");
        }
        else if(head->code.kind==IC_GOTO)
        {
            for(int i=8;i<26;i++)
            {
                if(RegList[i].use==1 && (RegList[i].op->kind==VARIABLE || RegList[i].op->kind==ADDRESS))
                {
                    fprintf(fp2,"  sw %s, %d($fp)\n",reg_name[i],mem_offset(RegList[i].op));
                }
            }
            fprintf(fp2, "  j %s\n",head->code.u.gt.op_goto->u.lable_name);
            return block_tail->next;
        }
        else if(head->code.kind==IC_IFGOTO)
        {
            Operand op_x = head->code.u.ifgt.x;
		    Operand op_relop = head->code.u.ifgt.relop;
		    Operand op_y = head->code.u.ifgt.y;
		    Operand op_z = head->code.u.ifgt.z;
    
		    int r_x = Ensure(op_x,head);
		    int r_y = Ensure(op_y,head);

            for(int i=8;i<26;i++)
            {
                if(RegList[i].use==1 && RegList[i].op->kind==VARIABLE)
                {
                    fprintf(fp2,"  sw %s, %d($fp)\n",reg_name[i],mem_offset(RegList[i].op));
                }
            }

		    if(strcmp(op_relop->u.relop,"==")==0)
		    {
		    	fprintf(fp2, "  beq %s, %s, %s\n",reg_name[r_x], reg_name[r_y], op_z->u.lable_name);
		    }
		    else if(strcmp(op_relop->u.relop,"!=")==0)
		    {
		    	fprintf(fp2, "  bne %s, %s, %s\n",reg_name[r_x], reg_name[r_y], op_z->u.lable_name);		
		    }
		    else if(strcmp(op_relop->u.relop,">")==0)
		    {
		    	fprintf(fp2, "  bgt %s, %s, %s\n",reg_name[r_x], reg_name[r_y], op_z->u.lable_name);		
		    }
		    else if(strcmp(op_relop->u.relop,"<")==0)
		    {
		    	fprintf(fp2, "  blt %s, %s, %s\n",reg_name[r_x], reg_name[r_y], op_z->u.lable_name);		
		    }
		    else if(strcmp(op_relop->u.relop,">=")==0)
		    {
		    	fprintf(fp2, "  bge %s, %s, %s\n",reg_name[r_x], reg_name[r_y], op_z->u.lable_name);		
		    }
		    else if(strcmp(op_relop->u.relop,"<=")==0)
		    {
		    	fprintf(fp2, "  ble %s, %s, %s\n",reg_name[r_x], reg_name[r_y], op_z->u.lable_name);		
		    }
            else
            {
                printf("sorry to see u get here\n");
                exit(4);
            }
            return block_tail->next;
        }
        else if(head->code.kind==IC_READ)
        {
           	fprintf(fp2, "  addi $sp, $sp, -4\n");
		    fprintf(fp2, "  sw $ra, 0($sp)\n");
		    fprintf(fp2, "  jal read\n");
		    fprintf(fp2, "  lw $ra, 0($sp)\n");
		    fprintf(fp2, "  addi $sp, $sp, 4\n");
		    int r_x= Ensure(head->code.u.read.op_read,head);
		    fprintf(fp2, "  move %s, $v0\n", reg_name[r_x]);
        }
        else if(head->code.kind==IC_WRITE)
        {
            int reg_x = Ensure(head->code.u.write.op_write,head);
			fprintf(fp2, "  move $a0, %s\n", reg_name[reg_x]);
			fprintf(fp2, "  addi $sp, $sp, -4\n");
			fprintf(fp2, "  sw $ra, 0($sp)\n");
			fprintf(fp2, "  jal write\n");
			fprintf(fp2, "  lw $ra, 0($sp)\n");
			fprintf(fp2, "  addi $sp, $sp, 4\n");
        }
        else if(head->code.kind==IC_CALL)
        {
            fprintf(fp2, "  addi $sp, $sp, -4\n");
		    fprintf(fp2, "  sw $ra, 0($sp)\n");
		    fprintf(fp2, "  jal %s\n",head->code.u.call.f->u.func_name);
		    fprintf(fp2, "  lw $ra, 0($sp)\n");
		    //fprintf(fp2, "  addi $sp, $sp, 4\n");
            int count_arg=0;
            struct InterCodes* temp_head=head->prev;
            while(temp_head!=NULL && temp_head->code.kind==IC_ARG)
            {
                count_arg++;
                temp_head=temp_head->prev;
            }
            fprintf(fp2, "  addi $sp, $sp, %d\n",4*count_arg+4);
            int r_x= Ensure(head->code.u.call.x,head);
		    fprintf(fp2, "  move %s, $v0\n", reg_name[r_x]);
        }
        else if(head->code.kind==IC_ARG)
        {
            int r_x=Ensure(head->code.u.arg.x,head);
            fprintf(fp2, "  addi $sp, $sp, -4\n");
            fprintf(fp2, "  sw %s, 0($sp)\n",reg_name[r_x]);
        }
        else if(head->code.kind==IC_DEC)
        {
           
        }
        else if(head->code.kind==IC_PARAM)
        {
           
        }
        else if(head->code.kind==IC_ADDR)
        {
            int r_left = Ensure(head->code.u.addr.left,head);
    	    fprintf(fp2, "  la %s, %d($fp)\n", reg_name[r_left], mem_offset(head->code.u.addr.right));
        }
        else if(head->code.kind==IC_R_DEREF)
        {
            //x := *y lw reg(x), 0(reg(y))
		    int r_left = Ensure(head->code.u.r_deref.left, head);
    	    int r_right = Ensure(head->code.u.r_deref.right, head);
    	    fprintf(fp2, "  lw %s, 0(%s)\n", reg_name[r_left], reg_name[r_right]);
        }
        else if(head->code.kind==IC_L_DEREF)
        {
            //*x = y sw reg(y), 0(reg(x))
            int r_left = Ensure(head->code.u.l_deref.left, head);
    	    int r_right = Ensure(head->code.u.l_deref.right, head);
    	    fprintf(fp2, "  sw %s, 0(%s)\n", reg_name[r_right], reg_name[r_left]);
        }
        else
        {
            printf("why you get here?\n");
            exit(2);
        }
        head=head->next;
    }
    //TODO:所有修改过的变量都写回内存
    for(int i=8;i<26;i++)
    {
        if(RegList[i].use==1 && (RegList[i].op->kind==VARIABLE || RegList[i].op->kind==ADDRESS))
        {
            fprintf(fp2,"  sw %s, %d($fp)\n",reg_name[i],mem_offset(RegList[i].op));
        }
    }

    return block_tail->next;
}

//set offset in frame
int Init_func(struct InterCodes* head)
{
    head=head->next;
    int offset=-4;
    int param_offset=8;
    VarDescList->next=NULL;
    
    
    while(head!=NULL && head->code.kind!=IC_FUNC)
    {
        
        if(head->code.kind==IC_PARAM)
        {
            struct VarDesc* var_head=VarDescList;
            while(var_head->next!=NULL)
            {
                var_head=var_head->next;
            }
            var_head->next=malloc(sizeof(struct VarDesc));
            var_head->next->next=NULL;
            var_head->next->op=head->code.u.param.x;
            var_head->next->offset=param_offset;
            printf("%d %s\n",param_offset,Opname(head->code.u.param.x));
            param_offset+=4;
        }
        else if(head->code.kind==IC_DEC)
        {
            struct VarDesc* var_head=VarDescList;
            while(var_head->next!=NULL)
            {
                var_head=var_head->next;
            }
            var_head->next=malloc(sizeof(struct VarDesc));
            var_head->next->next=NULL;
            var_head->next->op=head->code.u.param.x;
            offset-=head->code.u.dec.size;
            var_head->next->offset=offset+4;
            printf("%d %s\n",var_head->next->offset,Opname(head->code.u.dec.x));
        }
        else
        {
            OpList oplist=get_oplist(head);
            while(oplist!=NULL)
            {
                if(oplist->op->kind==VARIABLE || oplist->op->kind==ADDRESS)
                    offset=insert_VarDescList(oplist->op,offset);
                oplist=oplist->next;
            }
        }
        head=head->next;
    }
    return offset;
    
}

int insert_VarDescList(Operand op,int cur_offset)
{
    struct VarDesc* head=VarDescList->next;
    while(head!=NULL)
    {
        if(Opcmp(op,head->op)==0)
        {
            return cur_offset;
        }
        head=head->next;
    }
    head=VarDescList;
    while(head->next!=NULL)
    {
        head=head->next;
    }
    head->next=malloc(sizeof(struct VarDesc));
    head->next->next=NULL;
    head->next->op=op;
    head->next->offset=cur_offset;
    printf("%d %s\n",cur_offset,Opname(op));


    cur_offset=cur_offset-4;
    
    return cur_offset;
}

int mem_offset(Operand op)
{
    struct VarDesc* head=VarDescList->next;
    while(head!=NULL)
    {
        if(Opcmp(op,head->op)==0)
        {
            return head->offset;
        }
        head=head->next;
    }
    printf("WTF???\n");
    exit(10);
}

void RefreshRegList()
{
    for(int i=0;i<32;i++)
    {
        if(i>=8&&i<=25)
        {
            RegList[i].use=0;
            RegList[i].op=NULL;
        }
    }
    RegList[0].use=1;//$0

    //$at、$k0、$k1这三个寄存器是专门预留给汇编器使用的
    RegList[1].use=1;//$at
    RegList[26].use=1;//$k0
    RegList[27].use=1;//$k1

    RegList[28].use=1;//$gp
    RegList[29].use=1;//$sp
    RegList[30].use=1;//$fp

    RegList[31].use=1;//$ra
}

void setBasicBlock(struct InterCodes* head)
{
    int block_count=0;
    int internal_count=1;
    while(head!=NULL)
    {
        if(head->code.kind==IC_LABLE || head->code.kind==IC_FUNC)
        {
            block_count++;
            internal_count=1;
        }
        else if(head->code.kind==IC_CALL || head->code.kind==IC_READ || head->code.kind==IC_WRITE)
        {
            block_count++;
            internal_count=1;
        }
        else if(head->prev->code.kind==IC_GOTO || head->prev->code.kind==IC_IFGOTO)
        {
            block_count++;
            internal_count=1;
        }
        
        head->code.InternalNo=internal_count;
        head->code.BasicBlock=block_count;
        internal_count++;
        if(head->code.kind==IC_CALL || head->code.kind==IC_READ || head->code.kind==IC_WRITE)
        {
            block_count++;
            internal_count=1;
        }
        head=head->next;
    }
}