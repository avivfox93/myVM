
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 1740 of yacc.c  */
#line 17 "ast.y"

#include "AssemblerGen.h"
#include <string.h>



/* Line 1740 of yacc.c  */
#line 47 "ast.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT_NUM = 258,
     REG = 259,
     FLOAT_NUM = 260,
     MLU = 261,
     MLUI = 262,
     SET = 263,
     LOAD = 264,
     STORE = 265,
     SHIFT_A = 266,
     SHIFT_L = 267,
     BRANCH = 268,
     SHIFTV_L = 269,
     SHIFTV_A = 270,
     ID = 271,
     STRING = 272,
     DATA_SEG = 273,
     CODE_SEG = 274,
     JUMP = 275,
     NOP = 276,
     LOADI = 277,
     SYSCALL = 278
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1740 of yacc.c  */
#line 23 "ast.y"

   int ival;
   // float numbers in the source program are stored as double
   double fval;
   enum op op;
   char name[100];
   char str[200];

   Command* command;
   
   std::stack<int>* lst;
   



/* Line 1740 of yacc.c  */
#line 103 "ast.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


