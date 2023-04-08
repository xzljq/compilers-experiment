#include <stdio.h>
#include "syntax.tab.h"
#include "senmatic.h"
#include "TreeNode.h"
#include "ir.h"
#include "debug.h"
#include "mips32.h"

extern struct TreeNode* root;
extern struct InterCodes* IC_head;
FILE* fp2;

int main(int argc, char** argv)
{ 
	#ifdef _DEBUG
		printf("DEBUG MODE\n");
	#endif
	if (argc <= 1) return 1;
	FILE* f = fopen(argv[1], "r");
	if (!f)
	{
		perror(argv[1]);
		return 1;
	}

	fp2 = fopen(argv[2], "w");
	if (!fp2)
	{
		perror(argv[2]);
		return 1;
	}

	yyrestart(f);
    //yydebug = 1;
	yyparse();

	senmatic_check(root);
	generate_ic(root);

	ProcessIR(IC_head);//delete some useless ir

	gen_mips(IC_head);

	Print_IR(IC_head);
	//writeToFile(IC_head,fp);

	return 0;
}