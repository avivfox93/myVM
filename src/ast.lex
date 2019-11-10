%{
/* ast.tab.h was generated by bison with the -d option */
#include "ast.tab.h"
#include "AssemblerGen.h"
extern int atoi (const char *);

/* columns  are not tracked here (yylloc.first_column and yylloc.last_column are not set) */
//#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; 
%}

%x COMMENT

%option noyywrap
/*  this will cause  flex to maintain the current input line number in 
    the global variable int yylineno.
*/	
%option yylineno
%%

"\'"[A-Za-z0-9]"\'" { yylval.ival = yytext[1]; return INT_NUM; }
"-"[0-9]+  { yylval.ival = -(atoi (yytext)); return INT_NUM; }
[0-9]+     { yylval.ival = atoi (yytext); return INT_NUM; }

[0-9]+\.[0-9]+ { yylval.fval = atof (yytext); return FLOAT_NUM; }

[\r\n\t ]+   /* skip white space */

"0x"[a-fA-F0-9]+ {yylval.ival = hexToInt(yytext+2); return INT_NUM; }

".DATA"		{ return DATA_SEG; }
".CODE"		{ return CODE_SEG; }

"add"		{ yylval.op = ADD; return MLU; }
"addi"		{ yylval.op = ADD; return MLUI; }
"mul"		{ yylval.op = MUL; return MLU; }
"muli"		{ yylval.op = MUL; return MLUI; }
"div"		{ yylval.op = DIV; return MLU; }
"divi"		{ yylval.op = DIV; return MLUI; }


"addf"		{ yylval.op = ADD; return MLUF; }
"addif"		{ yylval.op = ADD; return MLUIF; }
"mulf"		{ yylval.op = MUL; return MLUF; }
"mulif"		{ yylval.op = MUL; return MLUIF; }
"divf"		{ yylval.op = DIV; return MLUF; }
"divif"		{ yylval.op = DIV; return MLUIF; }

"and"		{ yylval.op = AND; return MLU; }
"andi"		{ yylval.op = AND; return MLUI; }
"or"		{ yylval.op = OR; return MLU; }
"ori"		{ yylval.op = OR; return MLUI; }
"xor"		{ yylval.op = XOR; return MLU; }
"xori"		{ yylval.op = XOR; return MLUI; }

"seq"		{ yylval.op = EQ; return SET; }
"sne"		{ yylval.op = NEQ; return SET; }
"slt"		{ yylval.op = LT; return SET; }
"sgt"		{ yylval.op = GT; return SET; }
"sge"		{ yylval.op = GE; return SET; }
"sle"		{ yylval.op = LE; return SET; }

"seqf"		{ yylval.op = EQ; return SETF; }
"snef"		{ yylval.op = NEQ; return SETF; }
"sltf"		{ yylval.op = LT; return SETF; }
"sgtf"		{ yylval.op = GT; return SETF; }
"sgef"		{ yylval.op = GE; return SETF; }
"slef"		{ yylval.op = LE; return SETF; }

"lb"		{ yylval.op = BYTE; return LOAD; }
"lh"		{ yylval.op = HALF; return LOAD; }
"lw"		{ yylval.op = WORD; return LOAD; }

"sb"		{ yylval.op = BYTE; return STORE; }
"sh"		{ yylval.op = HALF; return STORE; }
"sw"		{ yylval.op = WORD; return STORE; }

"sal"		{ yylval.op = LEFT; return SHIFT_A; }
"sar"		{ yylval.op = RIGHT; return SHIFT_A; }
"sll"		{ yylval.op = LEFT; return SHIFT_L; }
"sla"		{ yylval.op = RIGHT; return SHIFT_L; }

"salv"		{ yylval.op = LEFT; return SHIFTV_A; }
"sarv"		{ yylval.op = RIGHT; return SHIFTV_A; }
"sllv"		{ yylval.op = LEFT; return SHIFTV_L; }
"slav"		{ yylval.op = RIGHT; return SHIFTV_L; }

"li"		{ return LOADI; }

"syscall"	{ return SYSCALL; }

"j"			{ return JUMP; }

"nop"		{ return NOP; }

"beq"		{ yylval.op = EQ; return BRANCH; }
"bne"		{ yylval.op = NEQ; return BRANCH; }
"blt"		{ yylval.op = LT; return BRANCH; }
"bgt"		{ yylval.op = GT; return BRANCH; }
"bge"		{ yylval.op = GE; return BRANCH; }
"ble"		{ yylval.op = LE; return BRANCH; }

"$zero"		{ yylval.ival = _$zero; return REG; }
"$v"[0-1]	{ yylval.ival = _$v0 + atoi(yytext + 2); return REG; }
"$at"		{ yylval.ival = _$at; return REG; }
"$a"[0-3]	{ yylval.ival = _$a0 + atoi(yytext + 2); return REG; }
"$t"[0-7]	{ yylval.ival = _$t0 + atoi(yytext + 2); return REG; }
"$s"[0-7]	{ yylval.ival = _$s0 + atoi(yytext + 2); return REG; }
"$t"[8-9]	{ yylval.ival = _$t8 + atoi(yytext + 2) - 8; return REG; }
"$t"[0-1]	{ yylval.ival = _$k0 + atoi(yytext + 2); return REG; }
"$gp"		{ yylval.ival = _$gp; return REG; }
"$sp"		{ yylval.ival = _$sp; return REG; }
"$fp"		{ yylval.ival = _$fp; return REG; }
"$ra"		{ yylval.ival = _$ra; return REG; }

"$f"[0-9]		{ yylval.ival = _$f0 + atoi(yytext + 2); return REG; }
"$f"[1-2][0-9]	{ yylval.ival = _$f0 + atoi(yytext + 2); return REG; }
"$f"[3][0-1]	{ yylval.ival = _$f0 + atoi(yytext + 2); return REG; }

[()=:{}\[\],]      { return yytext[0]; }
      
\"(\\.|[^"\\])*\"  {  strcpy (yylval.str, yytext); yylval.str[strlen(yylval.str)-1] = 0; stringParser(yylval.str); return STRING; }
[a-zA-Z][A-Za-z_]*  {  strcpy (yylval.name, yytext); return ID; }

  /* C++ style comments: */
"//".*     /* skip comment */

  /* C style comments: */
"/*"       { BEGIN (COMMENT); }
<COMMENT>. /* skip comment */
<COMMENT>"*/" { BEGIN (0); }

.          { fprintf (stderr, "unrecognized token %d\n", yytext[0]); }

%%
