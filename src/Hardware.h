/*
 * Hardware.h
 *
 *  Created on: 1 Nov 2019
 *      Author: Aviv
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>

#define EXIT_FLAG		0
#define READY_FLAG		1

//#define MEMORY_OFFSET	0x40000000
#define MEMORY_OFFSET	0
#define CODE_OFFSET		0x01000000

#define CODE_SEGMENT_SIZE	128 //in KB
#define MEMORY_SIZE			512 //in KB

#define OUT_BUFFER_SIZE		2048
#define IN_BUFFER_SIZE		2048

#ifdef DEBUGGER
extern char OUT_BUFFER[];
extern char IN_BUFFER[];
#endif
extern uint8_t MEMORY_SEGMENT[];
extern uint32_t REGISTERS[];
extern uint32_t F_REGISTERS[];
extern uint32_t PC;
extern uint8_t RUNNING_FLAG;

uint32_t memoryToIndex(uint32_t address);
uint32_t codeToIndex(uint32_t address);

void terminate(void);

#endif /* HARDWARE_H_ */
