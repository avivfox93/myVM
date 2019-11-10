/*
 * Hardware.cpp
 *
 *  Created on: 1 Nov 2019
 *      Author: Aviv
 */

#include "Hardware.h"
#include "Registers.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUGGER
char OUT_BUFFER[OUT_BUFFER_SIZE];
char IN_BUFFER[IN_BUFFER_SIZE];
#endif

uint8_t MEMORY_SEGMENT[MEMORY_SIZE*1024] = {0};
uint32_t REGISTERS[32] = {0};
uint32_t F_REGISTERS[32] = {0};
uint32_t PC = CODE_OFFSET;
uint8_t RUNNING_FLAG = READY_FLAG;

uint32_t memoryToIndex(uint32_t address){
	return address - MEMORY_OFFSET;
}

uint32_t codeToIndex(uint32_t address){
	return address - CODE_OFFSET;
}

void terminate(void){
#ifdef DEBUGGER
	sprintf(OUT_BUFFER,"\n\n**terminated**\n\n");
#else
	fprintf(stdout,"\n\n**terminated**\n\n");
	fflush(stdout);
#endif
	RUNNING_FLAG = EXIT_FLAG;
}
