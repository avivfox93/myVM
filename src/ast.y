%code {
#include <stdio.h>
#include <string>
#include <stdarg.h>

  /* yylex () and yyerror() need to be declared here */
int yylex (void);
void yyerror (std::string s);

/* void emit (const char *format, ...);  */

// number of errors 
int errors;
uint32_t data_offset;
}

%code requires {
#include "AssemblerGen.h"
#include <string.h>
}

/* note: no semicolon after the union */
%union {
   int ival;
   float fval;
   enum op op;
   char name[100];
   char str[200];

   Command* command;
   
   std::stack<int>* lst;
   
}

%token <ival> INT_NUM REG FREG
%token <fval> FLOAT_NUM
%token <op> MLU MLUI MLUF MLUIF SET SETF LOAD STORE SHIFT_A SHIFT_L BRANCH SHIFTV_L SHIFTV_A
%token <name> ID
%token <str> STRING
%token DATA_SEG CODE_SEG JUMP NOP LOADI SYSCALL MOVF LOADF STOREF
//type commands 

%type <lst> num_list

%error-verbose

%%

program: DATA_SEG data_labels CODE_SEG commands

commands: commands MLU REG ',' REG ',' REG { addCommand(createMlu($2,$3,$5,$7)); }
		| commands MLUI REG ',' REG ',' INT_NUM { addCommand(createMlui($2,$3,$5,$7)); }
		| commands SET REG ',' REG ',' REG ',' REG { addCommand(new Set($3,$5,$7,toOp($2))); }
		| commands LOAD REG ',' REG ',' INT_NUM { addCommand(createLoad($2,$3,$5,$7)); }
		| commands STORE REG ',' REG ',' INT_NUM { addCommand(createStore($2,$3,$5,$7)); }
		| commands SHIFT_A REG ',' REG ',' INT_NUM { addCommand(new ShiftImmediate($3,$5,$7,toOp($2),Command::SIGNED)); }
		| commands SHIFT_L REG ',' REG ',' INT_NUM { addCommand(new ShiftImmediate($3,$5,$7,toOp($2),Command::UNSIGNED)); }
		| commands SHIFTV_A REG ',' REG ',' REG ',' { addCommand(new Shift($3,$5,$7,toOp($2),Command::SIGNED)); }
		| commands SHIFTV_L REG ',' REG ',' REG ',' { addCommand(new Shift($3,$5,$7,toOp($2),Command::UNSIGNED)); }
		| commands LOADI REG ',' INT_NUM{ addCommand(new LoadImmediate($3,$5)); }
		| commands LOADI REG ',' ID{ addCommand(new LoadImmediate($3,getDataLabelAddress($5))); }
		| commands MLUF FREG ',' FREG ',' REG { addCommand(createMluF($2,$3,$5,$7)); }
		| commands MLUIF FREG ',' FREG ',' FLOAT_NUM { addCommand(createMluiF($2,$3,$5,$7)); }
		| commands MLUIF FREG ',' FREG ',' INT_NUM { addCommand(createMluiF($2,$3,$5,(float)$7)); }
		| commands SETF REG ',' FREG ',' FREG ',' REG { addCommand(new SetF($3,$5,$7,toOp($2))); }
		| commands SYSCALL { addCommand(new Syscall()); }
		| commands JUMP INT_NUM { addCommand(new Jump($3)); }
		| commands JUMP ID {Jump* j = new Jump(0); j->setLabel(new std::string($3)); addCommand(j); }
		| commands NOP { addCommand(new Nop()); }
		| commands BRANCH REG ',' REG ',' INT_NUM { addCommand(new Branch($7,toOp($2),$3,$5)); }
		| commands BRANCH REG ',' REG ',' ID { Branch* b = new Branch(0,toOp($2),$3,$5); b->setLabel(new std::string($7)); addCommand(b); }
		| commands ID ':' { addCodeLabel($2); }
		| commands MOVF FREG ',' REG { addCommand(new MovF($3,$5,Command::VtoF)); }
		| commands MOVF REG ',' FREG { addCommand(new MovF($3,$5,Command::FtoV)); }
		| commands STOREF REG ',' FREG ',' INT_NUM { addCommand(new StoreF($3,$5,$7)); }
		| commands LOADF FREG ',' REG ',' INT_NUM { addCommand(new LoadF($5,$3,$7)); }
		| /* empty */

data_labels:
		  | data_labels ID '[' INT_NUM ']' ':' INT_NUM '{' num_list '}' { addDataLabel($2,$4,$7,$9); } 
		  | data_labels ID '[' INT_NUM ']' ':' INT_NUM { addDataLabel($2,$4,$7,nullptr); }
		  | data_labels ID   STRING { addStringLabel($2,$3); }
		  | /* empty */

num_list: INT_NUM { std::stack<int>* t = newList(); $$ = t; addData(t, $1); }
		| FLOAT_NUM { std::stack<int>* t = newList(); $$ = t; addData(t,*((uint32_t*)&($1))); }
		| FLOAT_NUM ',' num_list { std::stack<int>* t = newList(); $$ = t; addData($3,*((uint32_t*)&($1))); copyData(t,$3); }
		| INT_NUM ',' num_list { std::stack<int>* t = newList(); $$ = t; addData($3,$1); copyData(t,$3); }

%%
main (int argc, char **argv)
{
  extern FILE *yyin;
  FILE* out;
  if (argc != 3) {
     fprintf (stderr, "Usage: %s <input-file-name> <output-file-name>\n", argv[0]);
	 return 1;
  }
  yyin = fopen (argv [1], "r");
  if (yyin == NULL) {
       fprintf (stderr, "failed to open %s\n", argv[1]);
	   return 2;
  }
  
  errors = 0;
  yyparse ();
   
  if (errors > 0) {
      fprintf(stderr, "compilation failed\n");
	  return 3;
  }else{
  	out = fopen(argv[2],"wb");
  	genToFile(out);
  }
  
  fclose (yyin);
  fclose(out);
  return 0;
}

void yyerror (std::string s)
{
  extern int yylineno;  // defined by flex
  errors++;
  fprintf (stderr, "line %d: %s\n", yylineno, s.c_str ());
}

void errorMsg (const char *format, ...)
{
    va_list argptr;
	va_start (argptr, format);
	// all the arguments following 'format' are passed on to vfprintf
	vfprintf (stderr, format, argptr); 
	va_end (argptr);
	
	errors++;
} 

void warningMsg (const char *format, ...)
{
    va_list argptr;
	va_start (argptr, format);
	// all the arguments following 'format' are passed on to vfprintf
	vfprintf (stderr, format, argptr); 
	va_end (argptr);
} 




