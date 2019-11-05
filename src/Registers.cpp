/*
 * Registers.cpp
 *
 *  Created on: 4 Nov 2019
 *      Author: Aviv
 */

#include "Registers.h"
#define _$zero 	0
#define _$at 	1
#define _$v0 	2
#define _$v1 	3
#define _$a0 	4
#define _$a1 	5
#define _$a2 	6
#define _$a3 	7
#define _$t0 	8
#define _$t1 	9
#define _$t2 	10
#define _$t3 	11
#define _$t4 	12
#define _$t5 	13
#define _$t6 	14
#define _$t7 	15
#define _$s0	16
#define _$s1	17
#define _$s2	18
#define _$s3	19
#define _$s4	20
#define _$s5	21
#define _$s6	22
#define _$s7	23
#define _$t8	24
#define _$t9	25
#define _$k0	26
#define _$k1	27
#define _$gp	28
#define _$sp	29
#define _$fp	30
#define _$ra	31
const char* REG_NAME[] = {
		"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7",
		"$s0","$s1","$s2","$s3","$s4","$s5","$s5","$s6","$t8","$t9","$k0","$k1","$gp","$sp","$fp","$ra"
};
