/*
 * Utils.h
 *
 *  Created on: 3 Nov 2019
 *      Author: Aviv
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "Command.h"
#include <stdio.h>
#include <string>

#define STATUS_UPDATE		0
#define STATUS_EXIT			1
#define STATUS_READY		2
#define COMMAND_CONTINUE	7
#define COMMAND_NEXT		8
#define COMMAND_EXIT		9
#define COMMAND_RESET		10

#define MEMORY_FILE_NAME	"mem.bin"

void runReset(void);
size_t loadBinaryFromFile(const char* dir, Command* commands[]);
size_t loadDataSegmentFromFile(const char* dir, uint8_t* buffer);
void runCommands(Command* commands[]);
bool runCommand(Command* commands[]);
const char* toBin(uint32_t data);
void printStatus(FILE* out, uint8_t status);
void printDebuggerData(FILE* out);
std::string uint8ToHexString(uint8_t num);

void printDecompiledStrings(Command* commands[], size_t size);
std::string decompiledStrings(Command* commands[], size_t size);
void printDataSegment(const uint8_t* data, uint32_t size);
std::string decompiledDataSegment(const uint8_t* data, size_t size);


#endif /* UTILS_H_ */
