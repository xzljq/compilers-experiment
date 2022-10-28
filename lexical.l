%{
    /*#include部分 */
    #include<stdio.h>
    #include<stdlib.h>
    #include<ctype.h>
    #include<assert.h>
    #include "syntax.tab.h"
    #include "TreeNode.h"


    int yycolumn = 1;
    #define YY_USER_ACTION\
        yylloc.first_line = yylloc.last_line = yylineno;\
        yylloc.first_column = yycolumn;\
        yylloc.last_column = yycolumn + yyleng - 1;\
        yycolumn += yyleng;

    void setextra(char* name,char* extra);
    void setint(int num);
    void setfloat(float num);
    static int to_Hex(const char* str);
    static int to_Dec(const char* str);
    static int to_Oct(const char* str);
    int My_atoi(const char* str);
%}
%option yylineno
letter_ [a-zA-Z_]
digit [0-9]
digits {digit}+
ws (\t|" ")+
%%
\n {yycolumn = 1;}
{ws} {}
"//" {char c = input();while (c != '\n') c = input();}
"/*" {char last=input();char cur=input();while(!(last=='*'&&cur=='/')){last=cur;cur=input();}}
[1-9]{digit}*|0[0-7]*|0(x|X)[0-9a-fA-F]+ {setint(My_atoi(yytext));return INT;}
({digits}"."{digits})|(({digits}"."{digits}|{digits}"."|"."{digits})(e|E)[+-]?{digits}) {setfloat(atof(yytext));return FLOAT;}
";" {setextra("SEMI",yytext);return SEMI;}
"," {setextra("COMMA",yytext);return COMMA;}
"=" {setextra("ASSIGNOP",yytext);return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" {setextra("RELOP",yytext);return RELOP;}
"+" {setextra("PLUS",yytext);return PLUS;}
"-" {setextra("MINUS",yytext);return MINUS;}
"*" {setextra("STAR",yytext);return STAR;}
"/" {setextra("DIV",yytext);return DIV;}
"&&" {setextra("AND",yytext);return AND;}
"||" {setextra("OR",yytext);return OR;}
"." {setextra("DOT",yytext);return DOT;}
"!" {setextra("NOT",yytext);return NOT;}
"int"|"float" {setextra("TYPE",yytext);return TYPE;}
"(" {setextra("LP",yytext);return LP;}
")" {setextra("RP",yytext);return RP;}
"[" {setextra("LB",yytext);return LB;}
"]" {setextra("RB",yytext);return RB;}
"{" {setextra("LC",yytext);return LC;}
"}" {setextra("RC",yytext);return RC;}
"struct" {setextra("STRUCT",yytext);return STRUCT;}
"return" {setextra("RETURN",yytext);return RETURN;}
"if" {setextra("IF",yytext);return IF;}
"else" {setextra("ELSE",yytext);return ELSE;}
"while" {setextra("WHILE",yytext);return WHILE;}
{letter_}({letter_}|{digit})* {setextra("ID",yytext);return ID;}
. {printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}
%%
void setextra(char* name,char* extra)
{
    yylval = (struct TreeNode *)malloc(sizeof(struct TreeNode));
	yylval->line = yylineno;
	yylval->istoken = 1;
	strcpy(yylval->name,name);
	strcpy(yylval->extra,extra);
	yylval->childnum = 0;
}

void setint(int num)
{
    yylval = (struct TreeNode *)malloc(sizeof(struct TreeNode));
	yylval->line = yylineno;
	yylval->istoken = 1;
	strcpy(yylval->name,"INT");
	yylval->INT=num;
	yylval->childnum = 0;
}

void setfloat(float num)
{
    yylval = (struct TreeNode *)malloc(sizeof(struct TreeNode));
	yylval->line = yylineno;
	yylval->istoken = 1;
	strcpy(yylval->name,"FLOAT");
	yylval->FLOAT=num;
	yylval->childnum = 0;
}

static int to_Hex(const char* str)//十六进制函数
{
	//十六进制只有  0123456789abcdef/ABCDEF
	int sum = 0;
	while (isxdigit(*str))//isxdigit()判断是否为十六进制数
	{
		if (isdigit(*str)) //判断是否为数字
		{
			sum = sum * 16 + *str - '0';
		}
		else
		{
			sum = sum * 16 + tolower(*str) - 'a' + 10;
		}
		str++;
	}
	return sum;
}
static int to_Dec(const char* str) //十进制函数
{
	//十进制的话只有数字：0123456789
	int sum = 0;
	while (isdigit(*str))
	{
		sum = sum * 10 + *str - '0';
		str++;
	}
	return sum;
}
static int to_Oct(const char* str)//八进制函数
{
	//八进制数字有：01234567
	int sum = 0;
	while (isdigit(*str) && *str != '8' && *str != '9')
	{
		sum = sum * 8 + *str - '0';
		str++;
	}
	return sum;
}
//前加static表示是静态函数，只能在本文件调用，不能在同工程中，其它C文件调用使用static，可以避免不想被其它模块调用的函数调用
int My_atoi(const char* str)
{
	assert(str != NULL);
	if (str == NULL)
	{
		return 0;
	}
	int index = 1;//正负标记
	int sum = 0;
	while (isspace(*str))
	{
		str++;
	}
	if (*str == '-')
	{
		index = -index;
		str++;
	}
	if (*str == '+')
	{
		str++;
	}
	if (*str == '0')
	{
		if (*(str + 1) == 'x' || *(str + 1) == 'X')
		{
			sum = to_Hex(str + 2);//十六进制→存储方式‘0x18’
		}
		else
		{
			sum = to_Oct(str + 1);//八进制→存储方式‘018’
		}
	}
	else
	{
		sum = to_Dec(str);
	}
	return sum * index;
}
