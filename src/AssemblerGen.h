/*
 * AssemblerGen.h
 *
 *  Created on: 6 Nov 2019
 *      Author: Aviv
 */

#ifndef ASSEMBLERGEN_H_
#define ASSEMBLERGEN_H_

#include <stack>
#include <string>
#include <string.h>
#include <map>
#include "Hardware.h"
#include "Command.h"
#include <math.h>

extern uint32_t codeAddress;

enum op{
	ADD,MUL,AND,OR,XOR,EQ,NEQ,LT,GT,GE,LE,WORD,HALF,BYTE,RIGHT,LEFT
};

Command::BinaryOp toOp(enum op op);


/*
 * Executable file Structure:
 * 32bits value of static data size in bytes
 * static data
 * code
 */
void genToFile(FILE* file);

std::stack<int>* newList();

void addDataLabel(std::string name,uint32_t length, uint8_t size, std::stack<int>* lst);

void addStringLabel(std::string name, const char* str);

void addData(std::stack<int>* s, int num);

void copyData(std::stack<int>* to, std::stack<int>* from);

void addCodeLabel(std::string id);

//ADD,MUL,AND,OR
Command* createMlu(enum op op, uint8_t save, uint8_t reg1, uint8_t reg2);

Command* createMlui(enum op op, uint8_t save, uint8_t reg, int16_t value);

Command* createLoad(enum op op, uint8_t to, uint8_t from, int16_t offset);

Command* createStore(enum op op, uint8_t to, uint8_t from, int16_t offset);

uint32_t getCodeLabelAddress(std::string id);

uint32_t getDataLabelAddress(std::string id);

int16_t getCodeLabelOffset(std::string id, uint32_t currentAddress);

void addCommand(Command* command);

uint8_t charToInt(char c);

uint32_t hexToInt(const char* str);

void stringParser(char* str);

#endif /* ASSEMBLERGEN_H_ */
