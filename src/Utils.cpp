/*
 * Utils.cpp
 *
 *  Created on: 3 Nov 2019
 *      Author: Aviv
 */
#include "Utils.h"
#include "Registers.h"
#include <iostream>
#include <stdio.h>

void runCommands(Command* commands[]){
	while(((PC - CODE_OFFSET)/4) < (CODE_SEGMENT_SIZE * 32)){
		runCommand(commands);
//		printf("CMD: %s, $v0=%d $a0=%d $t0=%d\n",toBin(commands[(PC - CODE_OFFSET)/4]->toBin()),REGISTERS[_$v0],REGISTERS[_$a0],REGISTERS[_$t0]);
	}
}

size_t loadDataSegmentFromFile(const char* dir, uint8_t* buffer){
	FILE* file;
	uint32_t dataSize = 0;
	file = fopen(dir,"rb");
	fread((void*)&dataSize,4,1,file);
	if(dataSize)
		fread((void*)buffer,1,dataSize,file);
	fclose(file);
	return dataSize;
}

size_t loadCodeFromFile(const char* dir, Command* commands[]){
	FILE* file;
	size_t pos;
	size_t idx = 0;
	uint32_t data,dataSize;
	file = fopen(dir,"rb");
	fread((void*)&dataSize,4,1,file);
	if(dataSize)
		fread((void*)MEMORY_SEGMENT,1,dataSize,file);
	while((pos = fread((void*)&data,4,1,file)) != 0)
		commands[idx++] = commandFromBin(data);
	fclose(file);
	return idx;
}

size_t loadBinaryFromFile(const char* dir, Command* commands[]){
	FILE* file;
	size_t pos;
	size_t idx = 0;
	uint32_t data,dataSize;
	file = fopen(dir,"rb");
	fread((void*)&dataSize,4,1,file);
	if(dataSize)
		fread((void*)MEMORY_SEGMENT,1,dataSize,file);
	while((pos = fread((void*)&data,4,1,file)) != 0)
		commands[idx++] = commandFromBin(data);
	while(idx < (CODE_SEGMENT_SIZE * 32))
		commands[idx++] = new Nop();
	fclose(file);
	return idx;
}

char buffer[33] = {0};
const char* toBin(uint32_t data){
	for(int i = 31 ; i >= 0 ; i--){
		buffer[i] = data%2 + '0';
		data /=2;
	}
	return buffer;
}

void runReset(void){
	RUNNING_FLAG = READY_FLAG;
	PC = CODE_OFFSET;
	for(int i = 0 ; i < 32 ; i++){
		REGISTERS[i] = 0;
		F_REGISTERS[i] = 0;
	}
	REGISTERS[_$sp] = MEMORY_SIZE*1024 + MEMORY_OFFSET - 1;
	for(int i = 0 ; i < MEMORY_SIZE*1024 ; i++)
		MEMORY_SEGMENT[i] = 0;
}

bool runCommand(Command* commands[]){
	if(((PC - CODE_OFFSET)/4) >= (CODE_SEGMENT_SIZE * 32) || RUNNING_FLAG == EXIT_FLAG)
		return false;
	commands[(PC - CODE_OFFSET)/4]->run();
	fflush(stdout);
	PC+=4;
	REGISTERS[_$zero] = 0;
	return true;
}

void printStatus(FILE* out, uint8_t status){
	fprintf(out, "%d ", STATUS_UPDATE);
}

void printDebuggerData(FILE* out){
	printStatus(out, STATUS_UPDATE);
	fprintf(out, "%d %d",CODE_OFFSET, MEMORY_OFFSET);
	for(int i = 0 ; i < 32 ; i++)
		fprintf(out," %d",REGISTERS[i]);
	for(int i = 0 ; i < 32 ; i++)
		fprintf(out," %d",*((uint32_t*)(&F_REGISTERS[i])));
	fprintf(out," %d\n",PC);
	FILE* file = fopen(MEMORY_FILE_NAME,"w");
	fwrite(MEMORY_SEGMENT,1,MEMORY_SIZE*1024,file);
	fclose(file);
}

std::string uint8ToHexString(uint8_t num){
	std::stringstream ss;
	char low = num % 16;
	char high = (num/16)%16;
	low += low < 10 ? '0' : 'A' - 10;
	high += high < 10 ? '0' : 'A' - 10;
	ss << "0x" << high << low;
	return ss.str();
}

void printDataSegment(const uint8_t* data, uint32_t size){
	printf(".DATA:\n");
	if(!size)
		return;
	printf("A[%d]:8 = [",size);
	printf("%s",uint8ToHexString(data[0]).c_str());
	for(uint32_t i = 1 ; i < size ; i++){
		printf(",%s",uint8ToHexString(data[i]).c_str());
	}
	printf("]\n");
}

std::string decompiledStrings(Command* commands[], size_t size){
	std::stringstream ss;
	for(int i = 0 ; i < size ; i++)
		 ss <<commands[i]->toString() << "\n";
	return ss.str();
}

void printDecompiledStrings(Command* commands[], size_t size){
	printf(".CODE:\n");
	for(int i = 0 ; i < size ; i++)
		printf("%s\n",commands[i]->toString().c_str());
}


