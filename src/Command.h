/*
 * Command.h
 *
 *  Created on: 1 Nov 2019
 *      Author: Aviv
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdint.h>
#include <stdio.h>
#include "Hardware.h"
#include "Registers.h"
#include <string>
#include <sstream>

//#define SIZE_BYTE	0
//#define SIZE_HALF	1
//#define SIZE_WORD	2

#define OP_NOP		0x00
#define OP_JUMP		0x01
#define OP_BRANCH	0x02
#define OP_ADD		0x03
#define OP_ADDI		0x04
#define OP_AND		0x05
#define OP_ANDI		0x06
#define OP_OR		0x07
#define OP_ORI		0x08
#define OP_XOR		0x09
#define OP_XORI		0x0A
#define OP_LOAD		0x0B
#define OP_STORE	0x0C
#define OP_SET		0x0D
#define OP_SHIFT	0x0E
#define OP_SHIFTI	0x0F
#define OP_LOADI	0x10
#define OP_MUL		0x11
#define OP_MULI		0x12
#define OP_SYSCALL	0x1F

#define JUMP_MASK		0x07ffffff
#define REGISTER_MASK	0x0000001f
#define OPCODE_MASK		0x0000001f
#define OFFSET_MASK		0x00007fff

#ifdef DEBUGGER
#include <jni.h>
#include "jni_md.h"
void jniCallbackInit(JNIEnv * env, jobject o);
void jniCallbackUpdate(JNIEnv * env, jobject o);
void javaImitInputCallback();
#endif

uint32_t jumpBin(uint32_t address);
uint32_t mluBin(uint8_t sign, uint8_t reg1, uint8_t reg2, uint8_t save, uint8_t opcode);
uint32_t mluiBin(uint8_t sign,uint16_t data, uint8_t reg, uint8_t save, uint8_t opcode);
uint32_t shiftBin(uint8_t sign, uint8_t dir, uint8_t reg1, uint8_t reg2, uint8_t save, uint8_t opcode);
uint32_t shiftiBin(uint8_t sign, uint8_t dir, uint8_t data, uint8_t reg, uint8_t save, uint8_t opcode);
uint32_t loadBin(uint16_t offset, uint8_t size, uint8_t from, uint8_t to, uint8_t opcode);
uint32_t storeBin(uint16_t offset, uint8_t size, uint8_t from, uint8_t to, uint8_t opcode);
uint32_t setBin(uint8_t type, uint8_t reg1, uint8_t reg2, uint8_t save, uint8_t opcode);
uint32_t branchBin(int16_t lines, uint8_t type, uint8_t reg1, uint8_t reg2, uint8_t opcode);
uint32_t loadiBin(uint32_t data, uint8_t reg, uint8_t opcode);

struct jump{
	uint32_t address:	27;
};
//Xor(uint8_t reg1, uint8_t reg2, uint8_t save)


struct mlu{
	uint8_t sign:	1;
	uint16_t 	:	11;
	uint8_t reg1:	5;
	uint8_t reg2:	5;
	uint8_t save:	5;
};
//XorImmediate(uint8_t save, uint8_t reg, uint16_t data)

struct mlui{
	uint8_t sign:	1;
	int16_t data:	16;
	uint8_t reg:	5;
	uint8_t save:	5;
};
//Shift(uint8_t save, uint8_t reg1, uint8_t reg2, enum DIRECTION dir, enum SIGN sign)


struct shift{
	uint16_t 	:	10;
	uint8_t sign:	1;
	uint8_t dir:	1;
	uint8_t reg1:	5;
	uint8_t reg2:	5;
	uint8_t save:	5;
};
//ShiftImmediate(uint8_t save, uint8_t reg, uint8_t shift, enum DIRECTION dir, enum SIGN sign)


struct shifti{
	uint8_t 	:	5;
	uint8_t sign:	1;
	uint8_t dir:	1;
	uint8_t 	:	5;
	uint8_t reg:	5;
	uint8_t save:	5;
	uint8_t data:	5;
};
//LoadByte(uint8_t from, uint8_t to, int16_t offset)


struct load{
	int16_t	offset:	15;
	uint8_t size:	2;
	uint8_t from:	5;
	uint8_t	to:		5;
};
//StoreWord(uint8_t addressReg, uint8_t dataReg, int16_t offset)


struct store{
	int16_t	offset:	15;
	uint8_t size:	2;
	uint8_t from:	5;
	uint8_t	to:		5;
};
//Set(uint8_t reg1, uint8_t reg2, uint8_t save, enum TYPE type)

struct set{
	uint8_t type: 	3;
	uint16_t 	:	9;
	uint8_t reg1: 	5;
	uint8_t reg2: 	5;
	uint8_t save:	5;
};

struct branch{
	int16_t lines:	14;
	uint8_t type:	3;
	uint8_t reg1:	5;
	uint8_t reg2:	5;
};
//LoadImmediate(uint8_t reg, uint32_t value)

struct loadi{
	uint32_t data:	22;
	uint8_t reg:	5;
};

struct binCommand{
	uint32_t	data:	27;
	uint8_t 	opcode:	5;
};

union binData{
	struct binCommand comm;
	struct jump jump;
	struct mlu mlu;
	struct mlui mlui;
	struct shift shift;
	struct shifti shifti;
	struct load load;
	struct store store;
	struct set set;
	struct branch branch;
	struct loadi loadi;
};

class Command{
public:
	enum BinaryOp{
		EQ = 0,NEQ = 1,GT = 2,LT = 3,GE = 4,LE = 5, SIZE_BYTE = 6, SIZE_HALF = 7, SIZE_WORD = 8, LEFT = 9, RIGHT = 10
	};
	enum SIGN{
		UNSIGNED = 0, SIGNED = 1
	};
	Command(){};
	virtual void run(){};
	virtual uint32_t toBin(){return 0;};
	virtual std::string toString(){return "nop";};
	virtual ~Command(){};
	virtual std::string* getLabel(){return nullptr;}
	virtual void setLabel(std::string* label){}
};

class MluImmediate : public Command{
public:
	enum SIGN{
		UNSIGNED = 0, SIGNED = 1
	};
	MluImmediate(uint8_t save, uint8_t reg, uint16_t data, enum SIGN sign){
		_save = save; _reg = reg; _data = data; _sign = sign;
	}
	virtual uint32_t toBin(){
		return mluiBin((uint8_t)_sign,_data,_reg,_save,0);
	}
	virtual std::string toString(){
		std::stringstream ss;
		ss << " " << REG_NAME[_save] << "," << REG_NAME[_reg] << "," << _data;
		return ss.str();
	}
	virtual void run(){};
protected:
	uint8_t _save, _reg;
	uint16_t _data;
	enum SIGN _sign;
	friend class MulImmediate;
};

class Mlu : public Command{
public:
	enum SIGN{
		UNSIGNED = 0, SIGNED = 1
	};
	Mlu(uint8_t reg1, uint8_t reg2, uint8_t save){
		_reg1 = reg1; _reg2 = reg2; _save = save;
	}
	virtual void run(){};
	virtual std::string toString(){
		std::stringstream ss;
		ss << " " << REG_NAME[_save] << "," << REG_NAME[_reg1] << "," << REG_NAME[_reg2];
		return ss.str();
	}
	virtual uint32_t toBin(){
		return mluBin(0,_reg1,_reg2,_save,0);
	}
protected:
	uint8_t _reg1, _reg2, _save;
	friend class Mul;
};

class Mul : public Mlu{
public:
	Mul(uint8_t reg1, uint8_t reg2, uint8_t save, Mlu::SIGN sign) : Mlu(reg1, reg2, save){
		_sign = sign;
	};
	virtual void run(){
		switch((Mlu::SIGN)_sign){
		case UNSIGNED:
			REGISTERS[_save] = REGISTERS[_reg1] * REGISTERS[_reg2];
			break;
		case SIGNED:
			REGISTERS[_save] = (int32_t)REGISTERS[_reg1] * (int32_t)REGISTERS[_reg2];
			break;
		}
	}
	virtual std::string toString(){
		std::string str = Mlu::toString();
		std::stringstream ss;
		ss << "mul" << str;
		return ss.str();
	}
	virtual uint32_t toBin(){
		return (Mlu::toBin() | (OP_MUL << 27) | (_sign & 0x01));
	}
private:
	uint8_t _sign;
};

class MulImmediate : public MluImmediate{
public:
	MulImmediate(uint8_t save, uint8_t reg, uint16_t data, enum SIGN sign) : MluImmediate(save, reg, data, sign){}
	virtual void run(){
		switch((Command::SIGN)_sign){
		case UNSIGNED:
			REGISTERS[_save] = REGISTERS[_reg] * (uint16_t)_data;
			break;
		case SIGNED:
			REGISTERS[_save] = (int32_t)((int32_t)REGISTERS[_reg] * (int16_t)_data);
			break;
		}
	}
	virtual std::string toString(){
		std::string str = MluImmediate::toString();
		std::stringstream ss;
		ss << "muli" << str;
		return ss.str();
	}
	virtual uint32_t toBin(){
		return (MluImmediate::toBin() | (OP_MULI << 27) | (_sign & 0x01));
	}
};

class Jump : public Command{
public:
	Jump(uint32_t address){
		_address = address;
	}
	virtual void run(){
		PC = _address - 4;
	}
	virtual std::string toString(){
		std::stringstream ss;
		ss << "j " << _address;
		return ss.str();
	}
	virtual uint32_t toBin(){
		return jumpBin(_address);
	}
	virtual std::string* getLabel(){return _label;}
	virtual void setLabel(std::string* label){_label = label;}
	void setAddress(uint32_t address){_address = address;};
private:
	std::string* _label = nullptr;
	uint32_t _address;
};

class Branch : public Command{
public:
	Branch(int16_t lines, enum BinaryOp type, uint8_t reg1, uint8_t reg2){
		_lines = lines; _type = type; _reg1 = reg1; _reg2 = reg2;
	}
	virtual uint32_t toBin(){
		return branchBin(_lines,(uint8_t)_type,_reg1,_reg2,OP_BRANCH);
	}
	virtual std::string toString(){
		std::stringstream ss;
		std::string name;
		switch(_type){
		case EQ:
			name = "beq";
			break;
		case NEQ:
			name = "bne";
			break;
		case GT:
			name = "bgt";
			break;
		case LT:
			name = "blt";
			break;
		case GE:
			name = "bge";
			break;
		case LE:
			name = "ble";
			break;
		}
		ss << name <<" " << REG_NAME[_reg1] << "," << REG_NAME[_reg2] << "," << _lines;
		return ss.str();
	}
	virtual void run();
	virtual std::string* getLabel(){return _label;}
	virtual void setLabel(std::string* label){_label = label;}
	void setLines(uint32_t lines){_lines = lines;};
private:
	std::string* _label = nullptr;
	int16_t _lines;
	uint8_t _reg1 = 0,_reg2 = 0;
	enum BinaryOp _type;
};

class Add : public Mlu{
public:
	Add(uint8_t reg1, uint8_t reg2, uint8_t save) : Mlu(reg1,reg2,save){}
	virtual void run(){
		REGISTERS[_save] = REGISTERS[_reg1] + REGISTERS[_reg2];
	}
	virtual std::string toString(){
		std::string str = Mlu::toString();
		std::stringstream ss;
		ss << "add " << str;
		return ss.str();
	}
	virtual uint32_t toBin(){
		return (Mlu::toBin() | (OP_ADD << 27));
	}
};

class AddImmediate : public MluImmediate{
public:
	AddImmediate(uint8_t save, uint8_t reg, uint16_t data, enum SIGN sign) : MluImmediate(save,reg,data,sign){}
	virtual uint32_t toBin(){
		return MluImmediate::toBin() | (OP_ADDI << 27);
	}
	virtual std::string toString(){
		std::string str = MluImmediate::toString();
		std::stringstream ss;
		ss << "addi " << str;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_save] = REGISTERS[_reg] + ((_sign == SIGNED) ? _data : (int16_t)_data);
	}
};

class And : public Mlu{
public:
	And(uint8_t reg1, uint8_t reg2, uint8_t save) : Mlu(reg1,reg2,save){}
	virtual uint32_t toBin(){
		return Mlu::toBin() | (OP_AND << 27);
	}
	virtual std::string toString(){
		std::string str;
		std::stringstream ss;
		ss << "and " << str;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_save] = REGISTERS[_reg1] & REGISTERS[_reg2];
	}
};

class AndImmediate : public MluImmediate{
public:
	AndImmediate(uint8_t save, uint8_t reg, uint16_t data) : MluImmediate(save,reg,data,UNSIGNED){}
	virtual uint32_t toBin(){
		return MluImmediate::toBin() | (OP_ANDI << 27);
	}
	virtual std::string toString(){
		std::string str = MluImmediate::toString();
		std::stringstream ss;
		ss << "andi " << str;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_save] = REGISTERS[_reg] & _data;;
	}
};

class Or : public Mlu{
public:
	Or(uint8_t reg1, uint8_t reg2, uint8_t save) : Mlu(reg1,reg2,save){}
	virtual uint32_t toBin(){
		return Mlu::toBin() | (OP_OR << 27);
	}
	virtual std::string toString(){
		std::string str;
		std::stringstream ss;
		ss << "or " << str;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_save] = REGISTERS[_reg1] | REGISTERS[_reg2];
	}
};

class OrImmediate : public MluImmediate{
public:
	OrImmediate(uint8_t save, uint8_t reg, uint16_t data) : MluImmediate(save,reg,data,UNSIGNED){}
	virtual uint32_t toBin(){
		return MluImmediate::toBin() | (OP_ORI << 27);
	}
	virtual std::string toString(){
		std::string str;
		std::stringstream ss;
		ss << "ori " << str;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_save] = REGISTERS[_reg] | _data;;
	}
};

class Xor : public Mlu{
public:
	Xor(uint8_t reg1, uint8_t reg2, uint8_t save) : Mlu(reg1,reg2,save){
		_reg1 = reg1; _reg2 = reg2; _save = save;
	}
	virtual uint32_t toBin(){
		return Mlu::toBin() | (OP_XOR << 27);
	}
	virtual std::string toString(){
		std::string str;
		std::stringstream ss;
		ss << "xor " << str;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_save] = REGISTERS[_reg1] ^ REGISTERS[_reg2];
	}
private:
	uint8_t _reg1, _reg2, _save;
};

class XorImmediate : public MluImmediate{
public:
	XorImmediate(uint8_t save, uint8_t reg, uint16_t data) : MluImmediate(save,reg,data,UNSIGNED){};
	virtual uint32_t toBin(){
		return MluImmediate::toBin() | (OP_XORI << 27);
	}
	virtual std::string toString(){
		std::string str;
		std::stringstream ss;
		ss << "xori " << str;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_reg] = REGISTERS[_reg] ^ _data;;
	}
};

class Set : public Command{
public:
	Set(uint8_t save, uint8_t reg1, uint8_t reg2, enum BinaryOp type){
		_reg1 = reg1; _reg2 = reg2; _save = save; _type = type;
	}
	virtual uint32_t toBin(){
		return setBin((uint8_t)_type,_reg1,_reg2,_save,OP_SET);
	}
	virtual std::string toString(){
		std::stringstream ss;
		std::string name;
		switch(_type){
		case EQ:
			name = "seq";
			break;
		case NEQ:
			name = "sne";
			break;
		case GT:
			name = "sgt";
			break;
		case LT:
			name = "slt";
			break;
		case GE:
			name = "sge";
			break;
		case LE:
			name = "sle";
			break;
		default:
			name = "";
			break;
		}
		ss << name << " " << REG_NAME[_save] << "," << REG_NAME[_reg1] << "," << REG_NAME[_reg2];
		return ss.str();
	}
	virtual void run();

private:
	uint8_t _reg1, _reg2, _save;
	enum BinaryOp _type;
};

class Load : public Command{
public:
	Load(uint8_t from, uint8_t to, int16_t offset){
		_from = from; _to = to; _offset = offset;
	}
	virtual uint32_t toBin(){return 0;};
	virtual std::string toString(){
		std::stringstream ss;
		ss << REG_NAME[_to] << "," << REG_NAME[_from] << "," << _offset;
		return ss.str();
	}

protected:
	uint8_t _from,_to;
	int16_t _offset;
};

class LoadByte : public Load{
public:
	LoadByte(uint8_t from, uint8_t to, int16_t offset) : Load(from,to,offset){};
	virtual uint32_t toBin(){
		return loadBin(_offset,SIZE_BYTE - SIZE_BYTE,_from,_to,OP_LOAD);
	}
	virtual std::string toString(){
		std::string str = Load::toString();
		std::stringstream ss;
		ss << "lb " << str;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_to] = MEMORY_SEGMENT[memoryToIndex(REGISTERS[_from] + _offset)];
	}
};

class LoadHalf : public Load{
public:
	LoadHalf(uint8_t from, uint8_t to, int16_t offset) : Load(from,to,offset){};
	virtual uint32_t toBin(){
		return loadBin(_offset,SIZE_HALF - SIZE_BYTE,_from,_to,OP_LOAD);
	}
	virtual std::string toString(){
		std::string str = Load::toString();
		std::stringstream ss;
		ss << "lh " << str;
		return ss.str();
	}
	virtual void run(){
		uint16_t* ptr;
		ptr = (uint16_t*)&MEMORY_SEGMENT[memoryToIndex(REGISTERS[_from] + _offset)];
		REGISTERS[_to] = *ptr;
	}
};

class LoadWord : public Load{
public:
	LoadWord(uint8_t from, uint8_t to, int16_t offset) : Load(from,to,offset){};
	virtual uint32_t toBin(){
		return loadBin(_offset,SIZE_WORD - SIZE_BYTE,_from,_to,OP_LOAD);
	}
	virtual std::string toString(){
		std::string str = Load::toString();
		std::stringstream ss;
		ss << "lw " << str;
		return ss.str();
	}
	virtual void run(){
		uint32_t* ptr;
		ptr= (uint32_t*)&MEMORY_SEGMENT[memoryToIndex(REGISTERS[_from] + _offset)];
		REGISTERS[_to] = *ptr;
	}
};

class Store : public Command{
public:
	Store(uint8_t addressReg, uint8_t dataReg, int16_t offset){
		_addressReg = addressReg; _dataReg = dataReg; _offset = offset;
	}
	std::string toString(){
		std::stringstream ss;
		ss << REG_NAME[_addressReg] << "," << REG_NAME[_dataReg] << "," << _offset;
		return ss.str();
	}
protected:
	uint8_t _addressReg,_dataReg;
	int16_t _offset;
};

class StoreWord : public Store{
public:
	StoreWord(uint8_t addressReg, uint8_t dataReg, int16_t offset) : Store(addressReg,dataReg,offset){}
	virtual uint32_t toBin(){
		return storeBin(_offset,SIZE_WORD - SIZE_BYTE,_dataReg,_addressReg,OP_STORE);
	}
	virtual std::string toString(){
		std::string str = Store::toString();
		std::stringstream ss;
		ss << "sw " << str;
		return ss.str();
	}
	virtual void run(){
		uint32_t *ptr;
		ptr = (uint32_t*)&MEMORY_SEGMENT[memoryToIndex(REGISTERS[_addressReg] + _offset)];
		*ptr = REGISTERS[_dataReg];
	}
};

class StoreHalf : public Store{
public:
	StoreHalf(uint8_t addressReg, uint8_t dataReg, int16_t offset) : Store(addressReg,dataReg,offset){}
	virtual uint32_t toBin(){
		return storeBin(_offset,SIZE_HALF - SIZE_BYTE,_dataReg,_addressReg,OP_STORE);
	}
	virtual std::string toString(){
		std::string str = Store::toString();
		std::stringstream ss;
		ss << "sh " << str;
		return ss.str();
	}
	virtual void run(){
		uint16_t *ptr;
		ptr = (uint16_t*)&MEMORY_SEGMENT[memoryToIndex(REGISTERS[_addressReg] + _offset)];
		*ptr = REGISTERS[_dataReg];
	}
};

class StoreByte : public Store{
public:
	StoreByte(uint8_t addressReg, uint8_t dataReg, int16_t offset) : Store(addressReg,dataReg,offset){}
	virtual uint32_t toBin(){
		return storeBin(_offset,SIZE_BYTE - SIZE_BYTE,_dataReg,_addressReg,OP_STORE);
	}
	virtual std::string toString(){
		std::string str = Store::toString();
		std::stringstream ss;
		ss << "sb " << str;
		return ss.str();
	}
	virtual void run(){
		uint8_t *ptr = &MEMORY_SEGMENT[memoryToIndex(REGISTERS[_addressReg] + _offset)];
		*ptr = REGISTERS[_dataReg];
	}
};

class Syscall : public Command{
public:
	Syscall(){};
	virtual uint32_t toBin(){
		return (OP_SYSCALL << 27);
	}
	std::string toString(){
		std::string str = "syscall";
		return str;
	}
	virtual void run();
};

class ShiftImmediate : public Command{
public:
	ShiftImmediate(uint8_t save, uint8_t reg, uint8_t shift, enum BinaryOp dir, enum SIGN sign){
		_save = save; _reg = reg; _shift = shift; _dir = dir; _sign = sign;
	}
	virtual uint32_t toBin(){
		return shiftiBin((uint8_t)_sign,(uint8_t)_dir,_shift,_reg,_save,OP_SHIFTI);
	}
	std::string toString(){
		std::stringstream ss;
		if(_sign == SIGNED)
			ss << "sa";
		else
			ss << "sl";
		switch(_dir){
		case LEFT:
			ss << "l";
			break;
		case RIGHT:
			ss << "r";
			break;
		}
		ss << " " << REG_NAME[_save] << "," << REG_NAME[_reg] << "," << _shift;
		return ss.str();
	}
	virtual void run();
private:
	uint8_t _reg, _shift, _save;
	enum BinaryOp _dir;
	enum SIGN _sign;
};

class Shift : public Command{
public:
	Shift(uint8_t save, uint8_t reg1, uint8_t reg2, enum BinaryOp dir, enum SIGN sign){
		_save = save; _reg1 = reg1; _reg2 = reg2; _dir = dir; _sign = sign;
	}
	virtual uint32_t toBin(){
		return shiftBin((uint8_t)_sign,(uint8_t)_dir,_reg1,_reg2,_save,OP_SHIFT);
	}
	std::string toString(){
		std::stringstream ss;
		if(_sign == SIGNED)
			ss << "sa";
		else
			ss << "sl";
		switch(_dir){
		case LEFT:
			ss << "l";
			break;
		case RIGHT:
			ss << "r";
			break;
		}
		ss << "v " << REG_NAME[_save] << "," << REG_NAME[_reg1] << "," << REG_NAME[_reg2];
		return ss.str();
	}
	virtual void run();
private:
	uint8_t _reg1, _reg2, _save;
	enum BinaryOp _dir;
	enum SIGN _sign;
};

class LoadImmediate : public Command{
public:
	LoadImmediate(uint8_t reg, uint32_t value){
		_reg = reg; _value = value;
	}
	virtual uint32_t toBin(){
		return loadiBin(_value,_reg,OP_LOADI);
	}
	std::string toString(){
		std::stringstream ss;
		ss << "li " << REG_NAME[_reg] << "," << _value;
		return ss.str();
	}
	virtual void run(){
		REGISTERS[_reg] = _value;
	}
private:
	uint8_t _reg;
	uint16_t _value;
};

class Nop : public Command{
public:
	Nop(){};
	virtual void run(){};
	std::string toString(){
		std::string str = "nop";
		return str;
	}
};

Command* commandFromBin(uint32_t data);

Jump* jumpFromBin(uint32_t data);
Branch* branchFromBin(uint32_t data);
Add* addFromBin(uint32_t data);
AddImmediate* addiFromBin(uint32_t data);
And* andFromBin(uint32_t data);
AndImmediate* andiFromBin(uint32_t data);
Or* orFromBin(uint32_t data);
OrImmediate* oriFromBin(uint32_t data);
Xor* xorFromBin(uint32_t data);
XorImmediate* xoriFromBin(uint32_t data);
Set* setFromBin(uint32_t data);
Command* loadFromBin(uint32_t data);
Command* storeFromBin(uint32_t data);
ShiftImmediate* shiftiFromBin(uint32_t data);
Shift* shiftFromBin(uint32_t data);
LoadImmediate* loadiFromBin(uint32_t data);


#endif /* COMMAND_H_ */
