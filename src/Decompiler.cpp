/*
 * Decompiler.cpp
 *
 *  Created on: 5 Nov 2019
 *      Author: Aviv
 */

#include "Decompiler.h"
#include "Utils.h"

uint8_t dataSegment[MEMORY_SIZE * 1028];

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr,"Usage: runtime filename");
		exit(EXIT_FAILURE);
	}
	Command** commands = new Command*[CODE_SEGMENT_SIZE * 32];
	loadBinaryFromFile(argv[1],commands);
	uint32_t size = loadDataSegmentFromFile(argv[1], dataSegment);
	printDataSegment(dataSegment,size);
	printDecompiledStrings(commands,CODE_SEGMENT_SIZE * 32);
	return 0;
}
