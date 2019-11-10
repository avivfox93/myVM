/*
 * Command.cpp
 *
 *  Created on: 1 Nov 2019
 *      Author: Aviv
 */


#include "Command.h"
#include <stdio.h>

#ifdef DEBUGGER
#include <cstring>
// Globals
jmethodID midStr;
const char * sigStr = "()Ljava/lang/String;";
JNIEnv* _static_env;
jobject _static_o;

// Init - One time to initialize the method id, (use an init() function)

void jniCallbackInit(JNIEnv *env, jobject o){
	jclass cl = env->GetObjectClass(o);
	midStr = env->GetMethodID(cl, "jniInputCallback", sigStr);
}

void jniCallbackUpdate(JNIEnv * env, jobject o){
	_static_env = env; _static_o = o;
}

// Methods
void javaImitInputCallback() {
  jstring string = _static_env->NewStringUTF(sigStr);
  jstring result = (jstring)_static_env->CallObjectMethod(_static_o, midStr, string);
  const char* res = _static_env->GetStringUTFChars(result, NULL);
  strcpy(IN_BUFFER,res);
}
#endif

uint32_t jumpBin(uint32_t address){
	return (address & JUMP_MASK) | ((OP_JUMP&OPCODE_MASK) << 27);
}

uint32_t mluBin(uint8_t sign, uint8_t reg1, uint8_t reg2, uint8_t save, uint8_t opcode){
	return (sign & 0x01) | ((reg1&REGISTER_MASK) << 12) | ((reg2&REGISTER_MASK) << 17) | ((save&OPCODE_MASK) << 22) | ((opcode&OPCODE_MASK) << 27);
}

uint32_t mluFBin(uint8_t opcode, uint8_t save, uint8_t reg1, uint8_t reg2){
	return ((opcode&OPCODE_MASK) << 27) | ((save&REGISTER_MASK) << 22) | ((reg2&REGISTER_MASK) << 17) | ((reg1&REGISTER_MASK) << 12);
}

uint32_t mluiFBin(uint8_t opcode, uint8_t save, uint8_t reg, uint32_t value){
	return ((opcode&OPCODE_MASK) << 27) | ((save&REGISTER_MASK)<<22) | ((reg&REGISTER_MASK)<<17) | (((value >> 16)&0xffff) << 1);
}

uint32_t setF(uint8_t opcode, uint8_t type, uint8_t save, uint8_t reg1, uint8_t reg2){
	return ((opcode&OPCODE_MASK)<<27) | ((save&REGISTER_MASK)<<22) | ((reg2&&REGISTER_MASK)<<17) | ((reg1&REGISTER_MASK)<<12) | (type & 0x07);
}
uint32_t movF(uint8_t opcode, uint8_t dir,uint8_t f, uint8_t v){
	return ((opcode&OPCODE_MASK)<<27) | ((f&REGISTER_MASK)<<22) | ((v&REGISTER_MASK)<<17) | (dir&0x1);
}

uint32_t mluiBin(uint8_t sign,uint16_t data, uint8_t reg, uint8_t save, uint8_t opcode){
	return (sign & 0x01) | ((data&0xffff) << 1) | ((reg&REGISTER_MASK) << 17) | ((save&OPCODE_MASK) << 22) | ((opcode&OPCODE_MASK) << 27);
}

uint32_t shiftBin(uint8_t sign, uint8_t dir, uint8_t reg1, uint8_t reg2, uint8_t save, uint8_t opcode){
	return ((sign & 0x01) << 10) | ((dir & 0x01) << 11) | ((reg1&REGISTER_MASK) << 12) | ((reg2&REGISTER_MASK) << 17) | ((save&OPCODE_MASK) << 22) | ((opcode&OPCODE_MASK) << 27);
}

uint32_t shiftiBin(uint8_t sign, uint8_t dir, uint8_t data, uint8_t reg, uint8_t save, uint8_t opcode){
	return ((sign & 0x01) << 5) | ((dir & 0x01) << 6) | ((reg&REGISTER_MASK) << 12) | ((save&OPCODE_MASK) << 17) | ((data&OPCODE_MASK) << 22) | ((opcode&OPCODE_MASK) << 27);
}

uint32_t loadBin(uint16_t offset, uint8_t size, uint8_t from, uint8_t to, uint8_t opcode){
	return ((offset & OFFSET_MASK)) | ((size & 0x03) << 15) | ((from & OPCODE_MASK) << 17) | ((to & OPCODE_MASK) << 22) | ((opcode & OPCODE_MASK) << 27);
}

uint32_t storeBin(uint16_t offset, uint8_t size, uint8_t from, uint8_t to, uint8_t opcode){
	return ((offset & OFFSET_MASK)) | ((size & 0x03) << 15) | ((from & OPCODE_MASK) << 17) | ((to & OPCODE_MASK) << 22) | ((opcode & OPCODE_MASK) << 27);
}

uint32_t setBin(uint8_t type, uint8_t reg1, uint8_t reg2, uint8_t save, uint8_t opcode){
	return (type & 0x07) | ((reg1 & REGISTER_MASK) << 12) | ((reg2 & REGISTER_MASK) << 17) | ((save & OPCODE_MASK) << 22) | ((opcode & OPCODE_MASK) << 27);
}

uint32_t branchBin(int16_t lines, uint8_t type, uint8_t reg1, uint8_t reg2, uint8_t opcode){
	return (lines & 0x3fff) | ((type & 0x07) << 14) | ((reg1 & REGISTER_MASK) << 17) | ((reg2 & REGISTER_MASK) << 22) | ((opcode & OPCODE_MASK) << 27);
}

uint32_t loadiBin(uint32_t data, uint8_t reg, uint8_t opcode){
	return (data & 0x3fffff) | ((reg) << 22) | ((opcode & OPCODE_MASK) << 27);
}

Jump* jumpFromBin(uint32_t data){
	return new Jump((data << 5)>>5);
}
Branch* branchFromBin(uint32_t data){
	int16_t padding = 0;
	if((data >> 13)&1)//
		padding = 0xc000;
	return new Branch((int16_t)((data&0x3fff) | padding),(Branch::BinaryOp)((data >> 14)&0x7),(data >> 17)&REGISTER_MASK,(data >> 22)&REGISTER_MASK);
}
Add* addFromBin(uint32_t data){
	return new Add((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK);
}
AddImmediate* addiFromBin(uint32_t data){
	return new AddImmediate((data >> 22)&OPCODE_MASK,(data >> 17)&REGISTER_MASK,(data >> 1) & 0xffff,(AddImmediate::SIGN)(data&0x1));
}
And* andFromBin(uint32_t data){
	return new And((data >> 12)&REGISTER_MASK,((data >> 17)&REGISTER_MASK),((data >> 22)&OPCODE_MASK));
}
AndImmediate* andiFromBin(uint32_t data){
	return new AndImmediate((data >> 22)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 1)&0xffff);
}
Or* orFromBin(uint32_t data){
	return new Or((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK);
}
OrImmediate* oriFromBin(uint32_t data){
	return new OrImmediate((data >> 22)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 1)&0xffff);
}
Xor* xorFromBin(uint32_t data){
	return new Xor((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK);
}
XorImmediate* xoriFromBin(uint32_t data){
	return new XorImmediate((data >> 22)&OPCODE_MASK,(data >> 17)&REGISTER_MASK,(data >> 1)&0xffff);
}
Set* setFromBin(uint32_t data){
	return new Set((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK,(Command::BinaryOp)((data >> 3)&0x07));
}
Command* loadFromBin(uint32_t data){
	uint8_t from = (data >> 17)&REGISTER_MASK,to = (data >> 22)&REGISTER_MASK;
	uint16_t offset = data & 0x7fff;
	switch(((data >> 15) & 0x3) + Command::SIZE_BYTE){
	case Command::SIZE_BYTE:
		return new LoadByte(from,to,offset);
	case Command::SIZE_HALF:
		return new LoadHalf(from,to,offset);
	case Command::SIZE_WORD:
		return new LoadWord(from,to,offset);
	default:
		return 0;
	}
}
Command* storeFromBin(uint32_t data){
	uint8_t from = (data >> 17)&REGISTER_MASK,to = (data >> 22)&REGISTER_MASK;
	uint16_t offset = data & 0x7fff;
	switch(((data >> 15) & 0x3) + Command::SIZE_BYTE){
	case Command::SIZE_BYTE:
		return new StoreByte(to,from,offset);
	case Command::SIZE_HALF:
		return new StoreHalf(to,from,offset);
	case Command::SIZE_WORD:
		return new StoreWord(to,from,offset);
	default:
		return 0;
	}
}
ShiftImmediate* shiftiFromBin(uint32_t data){
	return new ShiftImmediate((data >> 17)&OPCODE_MASK,(data >> 12)&REGISTER_MASK,
			(data >> 22)&OPCODE_MASK,((data >> 6)&0x01)?Command::LEFT : Command::RIGHT,(ShiftImmediate::SIGN)((data >> 5)&0x01));
}
Shift* shiftFromBin(uint32_t data){
	return new Shift((data >> 22)&OPCODE_MASK,(data >> 12)&REGISTER_MASK,
			(data >> 17)&REGISTER_MASK,((data >> 11)&0x01)?Command::LEFT : Command::RIGHT,(Shift::SIGN)((data >> 10)&0x01));
}
LoadImmediate* loadiFromBin(uint32_t data){
	return new LoadImmediate((data >> 22)&REGISTER_MASK, data&0x3fffff);
}
Mul* mulFromBin(uint32_t data){
	return new Mul((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK, (Mlu::SIGN)(data&0x01));
}
MulImmediate* muliFromBin(uint32_t data){
	return new MulImmediate((data >> 22)&OPCODE_MASK,(data >> 17)&REGISTER_MASK,(data >> 1) & 0xffff,(MluImmediate::SIGN)(data&0x1));
}
Div* divFrombin(uint32_t data){
	return new Div((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK);
}
Divi* diviFromBin(uint32_t data){
	return new Divi((data >> 22)&OPCODE_MASK,(data >> 17)&REGISTER_MASK,(data >> 1) & 0xffff,(MluImmediate::SIGN)(data&0x1));
}
AddF* addFFromBin(uint32_t data){
	return new AddF((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK);
}
AddiF* addiFFromBin(uint32_t data){
	return new AddiF((data >> 22)&OPCODE_MASK,(data >> 17)&REGISTER_MASK,((data >> 1) & 0xffff) << 16);
}
MulF* mulFFromBin(uint32_t data){
	return new MulF((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK);
}
MuliF* muliFFromBin(uint32_t data){
	return new MuliF((data >> 22)&OPCODE_MASK,(data >> 17)&REGISTER_MASK,((data >> 1) & 0xffff) << 16);
}
DivF* divFFromBin(uint32_t data){
	return new DivF((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK);
}
DiviF* diviFFromBin(uint32_t data){
	return new DiviF((data >> 22)&OPCODE_MASK,(data >> 17)&REGISTER_MASK,((data >> 1) & 0xffff) << 16);
}
SetF* setFFromBin(uint32_t data){
	return new SetF((data >> 12)&REGISTER_MASK,(data >> 17)&REGISTER_MASK,(data >> 22)&OPCODE_MASK,(Command::BinaryOp)((data >> 3)&0x07));
}

Command* commandFromBin(uint32_t data){
	switch(data >> 27){
		case OP_ADD:
			return addFromBin(data);
		case OP_ADDI:
			return addiFromBin(data);
		case OP_AND:
			return andFromBin(data);
		case OP_ANDI:
			return andiFromBin(data);
		case OP_BRANCH:
			return branchFromBin(data);
		case OP_JUMP:
			return jumpFromBin(data);
		case OP_LOAD:
			return loadFromBin(data);
		case OP_NOP:
			return new Nop();
		case OP_OR:
			return orFromBin(data);
		case OP_ORI:
			return oriFromBin(data);
		case OP_SET:
			return setFromBin(data);
		case OP_STORE:
			return storeFromBin(data);
		case OP_XOR:
			return xorFromBin(data);
		case OP_XORI:
			return xoriFromBin(data);
		case OP_SHIFT:
			return shiftFromBin(data);
		case OP_SHIFTI:
			return shiftiFromBin(data);
		case OP_LOADI:
			return loadiFromBin(data);
		case OP_SYSCALL:
			return new Syscall();
		case OP_MUL:
			return mulFromBin(data);
		case OP_MULI:
			return muliFromBin(data);
		case OP_DIV:
			return divFrombin(data);
		case OP_DIVI:
			return diviFromBin(data);
		case OP_ADDF:
			return addFFromBin(data);
		case OP_ADDIF:
			return addiFFromBin(data);
		case OP_MULF:
			return mulFFromBin(data);
		case OP_MULIF:
			return muliFFromBin(data);
		case OP_DIVF:
			return divFFromBin(data);
		case OP_DIVIF:
			return diviFFromBin(data);
		case OP_SETF:
			return setFFromBin(data);
	}
	return new Nop();
}

void Branch::run(){
	bool taken = false;
	switch(_type){
	case EQ:
		if(REGISTERS[_reg1] == REGISTERS[_reg2])
			taken = true;
		break;
	case NEQ:
		if(REGISTERS[_reg1] != REGISTERS[_reg2])
			taken = true;
		break;
	case GT:
		if(REGISTERS[_reg1] > REGISTERS[_reg2])
			taken = true;
		break;
	case LT:
		if(REGISTERS[_reg1] < REGISTERS[_reg2])
			taken = true;
		break;
	case GE:
		if(REGISTERS[_reg1] >= REGISTERS[_reg2])
			taken = true;
		break;
	case LE:
		if(REGISTERS[_reg1] <= REGISTERS[_reg2])
			taken = true;
		break;
	default:
		break;
	}
	if(taken)
		PC += (_lines - 1)*4;
}

void Set::run(){
	bool taken = false;
	switch(_type){
	case EQ:
		if(REGISTERS[_reg1] == REGISTERS[_reg2])
			taken = true;
		break;
	case NEQ:
		if(REGISTERS[_reg1] != REGISTERS[_reg2])
			taken = true;
		break;
	case GT:
		if(REGISTERS[_reg1] > REGISTERS[_reg2])
			taken = true;
		break;
	case LT:
		if(REGISTERS[_reg1] < REGISTERS[_reg2])
			taken = true;
		break;
	case GE:
		if(REGISTERS[_reg1] >= REGISTERS[_reg2])
			taken = true;
		break;
	case LE:
		if(REGISTERS[_reg1] <= REGISTERS[_reg2])
			taken = true;
		break;
	default:
		break;
	}
	REGISTERS[_save] = taken;
}

void Syscall::run(){
	switch(REGISTERS[2]){
	case 1:
#ifdef DEBUGGER
		sprintf(OUT_BUFFER,"%d",REGISTERS[4]);
#else
		printf("%d", REGISTERS[4]);
#endif
		break;
	case 2:
#ifdef DEBUGGER
		sprintf(OUT_BUFFER,"%f",F_REGISTERS[12]);
#else
		printf("%f", F_REGISTERS[12]);
#endif
		break;
	case 3:
#ifdef DEBUGGER
		sprintf(OUT_BUFFER,"%lf",*((double*)&REGISTERS[12]));
#else
		printf("%lf", *((double*)&REGISTERS[12]));
#endif
		break;
	case 4:
#ifdef DEBUGGER
		sprintf(OUT_BUFFER,"%s",(const char*)&MEMORY_SEGMENT[(uint32_t)REGISTERS[4]]);
#else
		printf("%s",(const char*)&MEMORY_SEGMENT[(uint32_t)REGISTERS[4]]);
#endif
		break;
	case 5:
#ifdef DEBUGGER
		javaImitInputCallback();
		sscanf(IN_BUFFER,"%d",&REGISTERS[2]);
#else
		scanf("%d",&REGISTERS[2]);
#endif
		break;
	case 6:
#ifdef DEBUGGER
		javaImitInputCallback();
		sscanf(IN_BUFFER,"%f",(float*)&F_REGISTERS[0]);
#else
		scanf("%f",(float*)&F_REGISTERS[0]);
#endif
		break;
	case 7:
#ifdef DEBUGGER
		javaImitInputCallback();
		sscanf(IN_BUFFER,"%lf",(float*)&F_REGISTERS[0]);
#else
		scanf("%lf",(float*)&F_REGISTERS[0]);
#endif
		break;
	case 8:
#ifdef DEBUGGER
		javaImitInputCallback();
		sscanf(IN_BUFFER,"%s",(char*)&MEMORY_SEGMENT[memoryToIndex(REGISTERS[4])]);
		if(strlen(IN_BUFFER) > REGISTERS[5]){
			MEMORY_SEGMENT[memoryToIndex(REGISTERS[4]) + REGISTERS[5]] = 0;
		}
#else
		fgets((char*)&MEMORY_SEGMENT[memoryToIndex(REGISTERS[4])],REGISTERS[5],stdin);
#endif
		break;
	case 10:
		terminate();
		break;
	case 11:
#ifdef DEBUGGER
		sprintf(OUT_BUFFER,"%c",(char)REGISTERS[4]);
#else
		printf("%c",(char)REGISTERS[4]);
#endif
		break;
	case 12:
		REGISTERS[2] = getchar();
		break;
	default:
		break;
	}
	fflush(stdout);
}

uint32_t getOnePadding(uint8_t shift){
	uint32_t result = 0;
	uint32_t temp = 0x80000000;
	for(int i = 0 ; i < shift ; i++){
		result |= temp;
		temp = temp >> 1;
	}
	return result;
}

void ShiftImmediate::run(){
	uint32_t mask = 0;
	switch(_dir){
	case RIGHT:
		if(_sign == SIGNED)
			mask = getOnePadding(_shift);
		REGISTERS[_save] = REGISTERS[_reg] >> _shift;
		break;
	case LEFT:
		if(_sign == SIGNED)
			mask = REGISTERS[_reg] & 0x80000000;
		REGISTERS[_save] = REGISTERS[_reg] << _shift;
		break;
	default:
		return;
	}
	REGISTERS[_save] |= mask;
}

void Shift::run(){
	uint32_t mask = 0;
	switch(_dir){
	case RIGHT:
		if(_sign == SIGNED)
			mask = getOnePadding(REGISTERS[_reg2]);
		REGISTERS[_save] = REGISTERS[_reg1] >> REGISTERS[_reg2];
		break;
	case LEFT:
		if(_sign == SIGNED)
			mask = REGISTERS[_reg1] & 0x80000000;
		REGISTERS[_save] = REGISTERS[_reg1] << REGISTERS[_reg2];
		break;
	default:
		return;
	}
	REGISTERS[_save] |= mask;
}

void SetF::run(){
	float *r1 = (float*)&F_REGISTERS[_reg1],*r2 = (float*)&F_REGISTERS[_reg2];
	uint8_t res = 0;
	switch(_type){
	case EQ:
		res = *r1 == *r2;
		break;
	case NEQ:
		res = *r1 != *r2;
		break;
	case GT:
		res = *r1 > *r2;
		break;
	case LT:
		res = *r1 < *r2;
		break;
	case GE:
		res = *r2 >= *r2;
		break;
	case LE:
		res = *r1 <= *r2;
		break;
	}
	REGISTERS[_save] = res;
}
