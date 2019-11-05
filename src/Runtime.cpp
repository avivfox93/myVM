//============================================================================
// Name        : Runtime.cpp
// Author      : Aviv Fox
// Version     :
//============================================================================

#include "Command.h"
#include "Registers.h"
#include "Utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <iostream>

Command** commands = new Command*[CODE_SEGMENT_SIZE * 32];
void run(Command* commands[]);

const char str[] = "Hello World!\n";

int main(int argc, char* argv[]) {
	if(argc != 2){
		fprintf(stderr,"Usage: runtime filename");
		exit(EXIT_FAILURE);
	}
	loadBinaryFromFile(argv[1],commands);
	run(commands);
	return 0;
}

void run(Command* commands[]){
	REGISTERS[_$sp] = CODE_SEGMENT_SIZE * 32 - 1;
	while(((PC - CODE_OFFSET)/4) < (CODE_SEGMENT_SIZE * 32) && RUNNING_FLAG != EXIT_FLAG){
		commands[(PC - CODE_OFFSET)/4]->run();
		fflush(stdout);
		PC+=4;
		REGISTERS[_$zero] = 0;
	}
}

