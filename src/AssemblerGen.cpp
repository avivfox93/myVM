/*
 * AssemblerGen.cpp
 *
 *  Created on: 7 Nov 2019
 *      Author: Aviv
 */


#include <stack>
#include <string>
#include <string.h>
#include <map>
#include "AssemblerGen.h"
#include "Hardware.h"
#include "Command.h"
#include <math.h>

static std::stack<int> numStack;
static std::map<std::string,uint32_t> labelCodeMap;
static std::map<std::string,uint32_t> labelDataMap;
static Command* commands[MEMORY_SIZE*32];

static uint32_t memAddress = MEMORY_OFFSET;
uint32_t codeAddress = CODE_OFFSET;

/*
 * just for static cast
 */
Jump* _j = new Jump(0);
Branch* _b = new Branch(0,Command::EQ,0,0);

extern int errors;

Command::BinaryOp toOp(enum op op){
	switch(op){
	case EQ:
		return Command::EQ;
	case NEQ:
		return Command::NEQ;
	case LT:
		return Command::LT;
	case GT:
		return Command::GT;
	case GE:
		return Command::GE;
	case LE:
		return Command::LE;
	case BYTE:
		return Command::SIZE_BYTE;
	case HALF:
		return Command::SIZE_HALF;
	case WORD:
		return Command::SIZE_WORD;
	default:
		errors++;
		return Command::EQ;
	}
}


/*
 * Executable file Structure:
 * 32bits value of static data size in bytes
 * static data
 * code
 */
void genToFile(FILE* file){
	uint32_t size = memAddress - MEMORY_OFFSET;
	printf("DATA size: %d\n",size);
	fwrite(&size,4,1,file);
	fwrite(MEMORY_SEGMENT,1,size,file);
	uint32_t numOfCommands = (codeAddress - CODE_OFFSET)/4;
	uint32_t data;
	for(uint32_t i = 0 ; i < numOfCommands ; i++){
		if(commands[i]->getLabel()!=nullptr){
			if(typeid(*commands[i]) == typeid(*_b)){
				int16_t offset = getCodeLabelOffset(*commands[i]->getLabel(),CODE_OFFSET+i*4);
				((Branch*)commands[i])->setLines(offset);
			}else if(typeid(*commands[i]) == typeid(*_j)){
				uint32_t address = getCodeLabelAddress(*commands[i]->getLabel());
				((Jump*)commands[i])->setAddress(address);
			}
		}
		data = commands[i]->toBin();
		fwrite(&data,4,1,file);
	}
}

std::stack<int>* newList(){
	return new std::stack<int>;
}

void addDataLabel(std::string name,uint32_t length, uint8_t size, std::stack<int>* lst){
	labelDataMap[name] = memAddress;
	uint32_t* address32 = (uint32_t*)MEMORY_SEGMENT;
	uint16_t* address16 = (uint16_t*)MEMORY_SEGMENT;
	if(lst == nullptr){
		memAddress += length*(size/8);
		return;
	}
	while(!lst->empty()){
		if(size == 16)
			address16[memAddress - MEMORY_OFFSET] = lst->top();
		else if(size == 32)
			address32[memAddress - MEMORY_OFFSET] = lst->top();
		else
			MEMORY_SEGMENT[memAddress - MEMORY_OFFSET] = lst->top();
		lst->pop();
		if(size == 16)
			memAddress += 2;
		else if(size == 32)
			memAddress += 4;
		else
			memAddress++;
	}
}

void addStringLabel(std::string name, const char* str){
	labelDataMap[name] = memAddress;
	if(str == NULL)
		return;
	memcpy((void*)&MEMORY_SEGMENT[memAddress - MEMORY_OFFSET],(const void*)str,strlen(str)-1);
	MEMORY_SEGMENT[memAddress - MEMORY_OFFSET + strlen(str)] = 0;
	memAddress += strlen(str)+1;
}

void addData(std::stack<int>* s, int num){
	s->push(num);
}

void copyData(std::stack<int>* to, std::stack<int>* from){
	while(!from->empty()){
		to->push(from->top());
		from->pop();
	}
}

void addCodeLabel(std::string id){
	labelCodeMap[id] = codeAddress;
}

//ADD,MUL,AND,OR
Command* createMlu(enum op op, uint8_t save, uint8_t reg1, uint8_t reg2){
	switch(op){
		case ADD:
			return new Add(reg1,reg2,save);
		case MUL:
			return new Mul(reg1,reg2,save,Mlu::SIGNED);
		case AND:
			return new And(reg1,reg2,save);
		case OR:
			return new Or(reg1,reg2,save);
	}
	errors++;
	return new Nop();
}

Command* createMlui(enum op op, uint8_t save, uint8_t reg, int16_t value){
	switch(op){
		case ADD:
			return new AddImmediate(save,reg,value,MluImmediate::SIGNED);
		case MUL:
			return new MulImmediate(save,reg,value,MluImmediate::SIGNED);
		case AND:
			return new AndImmediate(save,reg,value);
		case OR:
			return new AndImmediate(save,reg,value);
	}
	errors++;
	return new Nop();
}

Command* createMluF(enum op op, uint8_t save, uint8_t reg1, uint8_t reg2){
	switch(op){
		case ADD:
			return new AddF(reg1,reg2,save);
		case MUL:
			return new MulF(reg1,reg2,save);
	}
	errors++;
	return new Nop();
}

Command* createMluiF(enum op op, uint8_t save, uint8_t reg, float value){
	uint32_t *tmp = (uint32_t*)&value;
	switch(op){
		case ADD:
			return new AddiF(save,reg,*tmp);
		case MUL:
			return new MuliF(save,reg,*tmp);
	}
	errors++;
	return new Nop();
}

Command* createLoad(enum op op, uint8_t to, uint8_t from, int16_t offset){
	switch(op){
	case BYTE:
		return new LoadByte(from,to,offset);
	case HALF:
		return new LoadHalf(from,to,offset);
	case WORD:
		return new LoadWord(from,to,offset);
	}
	errors++;
	return new Nop();
}

Command* createStore(enum op op, uint8_t to, uint8_t from, int16_t offset){
	switch(op){
	case BYTE:
		return new StoreByte(to,from,offset);
	case HALF:
		return new StoreHalf(to,from,offset);
	case WORD:
		return new StoreWord(to,from,offset);
	}
	errors++;
	return new Nop();
}

uint32_t getCodeLabelAddress(std::string id){
	std::map<std::string, uint32_t>::iterator it;
	uint32_t result = 0;
	it = labelCodeMap.find(id);
	if(it != labelCodeMap.end()){
		result = it->second;
	}else{
		errors++;
	}
	return result;
}

uint32_t getDataLabelAddress(std::string id){
	std::map<std::string, uint32_t>::iterator it;
	uint32_t result = 0;
	it = labelDataMap.find(id);
	if(it != labelCodeMap.end()){
		result = it->second;
	}else{
		errors++;
	}
	return result;
}

int16_t getCodeLabelOffset(std::string id, uint32_t currentAddress){
	std::map<std::string, uint32_t>::iterator it;
	int16_t result = 0;
	it = labelCodeMap.find(id);
	if(it != labelCodeMap.end()){
		result = (it->second - currentAddress)/4;
	}else{
		errors++;
	}
	return result;
}

void addCommand(Command* command){
	commands[(codeAddress - CODE_OFFSET)/4] = command;
	codeAddress += 4;
}

uint8_t charToInt(char c){
	if(c >= 'a' && c <= 'f')
		return c - 'a';
	else if(c >= 'A' && c <= 'F')
		return c - 'A';
	else if(c >= '0' && c <= '9')
		return c - '0';
	return 0;
}

uint32_t hexToInt(const char* str){
	uint32_t result = 0;
	int p = 0;
	for(int i = strlen(str) ; i >= 0 ; i--)
		result += (int)(charToInt(str[i])*pow(16,p++));
	return result;
}


void stringParser(char* str){
	char *ptr = strchr(str,'\\');
	if(ptr == NULL)
		return;
	switch(ptr[1]){
	case 'a':
		ptr[0] = '\a';
		break;
	case 'b':
		ptr[0] = '\b';
		break;
	case 'f':
		ptr[0] = '\f';
		break;
	case 'n':
		ptr[0] = '\n';
		break;
	case 'r':
		ptr[0] = '\r';
		break;
	case 't':
		ptr[0] = '\t';
		break;
	case 'v':
		ptr[0] = '\v';
		break;
	case '\\':
		ptr[0] = '\\';
		break;
	case '\'':
		ptr[0] = '\'';
		break;
	case '"':
		ptr[0] = '"';
		break;
	case '?':
		ptr[0] = '?';
		break;
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case'9':
		ptr[0] = atoi(ptr + 1);
		break;
	case 'x':
		break;
	case 'u':
		break;
	case 'U':
		break;
	}
	memcpy(ptr+1,ptr+2,strlen(ptr+1));
}

