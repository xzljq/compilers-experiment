#include<stdio.h>
#include"syntax.tab.h"
#include"senmatic.h"
#include"TreeNode.h"

extern struct TreeNode* root;

int main(int argc, char** argv)
{ 
	if (argc <= 1) return 1;
	FILE* f = fopen(argv[1], "r");
	if (!f)
	{
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
    //yydebug = 1;
	yyparse();

	senmatic_check(root);
	return 0;
}