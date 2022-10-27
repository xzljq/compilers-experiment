#ifndef _TREE_H
#define _TREE_H

struct TreeNode
{
	int line;//行号 语法单元并且该语法单元没有产生epsilon
	int istoken;
	char name[32];//语法单元/词法单元名称
	int INT;
	float FLOAT; 
	char extra[32];//额外打印:词法单元ID TYPE
	int childnum;
	struct TreeNode* childlist[7];
};
#endif