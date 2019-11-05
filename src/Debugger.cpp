#ifndef DEBUGGER
#define DEBUGGER
#endif

#include "Debugger.h"
#include "Command.h"
#include "Utils.h"
#include "Hardware.h"
#include "Registers.h"
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "jni_md.h"

Command** commands = new Command*[CODE_SEGMENT_SIZE * 32];
uint32_t codeSize = 0;

JNIEXPORT jboolean JNICALL Java_Debugger_begin(JNIEnv * env, jobject obj,
		jstring str) {
	return true;
}

/*
 * Class:     Debugger
 * Method:    decompileFile
 * Signature: ()V
 */
JNIEXPORT jstring JNICALL Java_Debugger_decompileFile(JNIEnv * env, jobject obj) {
	jstring result = env->NewStringUTF(decompiledStrings(commands,codeSize).c_str());
	return result;
}

/*
 * Class:     Debugger
 * Method:    updateMemory
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Debugger_updateMemory(JNIEnv * env, jobject obj){
	FILE* file = fopen(MEMORY_FILE_NAME, "r");
	fread(MEMORY_SEGMENT, 1, MEMORY_SIZE * 1024, file);
	fclose(file);
}

/*
 * Class:     Debugger
 * Method:    loadFile
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT int JNICALL Java_Debugger_loadFile(JNIEnv * env, jobject obj,
		jstring str) {
	jniCallbackInit(env,obj); // @suppress("Function cannot be resolved")
	runReset();
	const char* fileName = env->GetStringUTFChars(str, NULL);
	codeSize = loadCodeFromFile(fileName, commands);
	return codeSize;
}

/*
 * Class:     Debugger
 * Method:    runNext
 * Signature: ()V
 */
JNIEXPORT jstring JNICALL Java_Debugger_runNext(JNIEnv *env, jobject obj) {
	jniCallbackUpdate(env,obj); // @suppress("Function cannot be resolved")
	runCommand(commands);
	jstring result = env->NewStringUTF(OUT_BUFFER);
	OUT_BUFFER[0] = 0;
	return result;
}

/*
 * Class:     Debugger
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Debugger_stop(JNIEnv *env, jobject obj) {
	jniCallbackUpdate(env,obj); // @suppress("Function cannot be resolved")
	runReset();
}

/*
 * Class:     Debugger
 * Method:    getRegisters
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_Debugger_getRegisters(JNIEnv *env,
		jobject obj) {
	jintArray result;
	result = env->NewIntArray((jint) 65);
	jint fill[65];
	if (result == NULL)
		return NULL; /* out of memory error thrown */
	for (int i = 0; i < 32; i++)
		fill[i] = REGISTERS[i];
	for (int i = 0; i < 32; i++)
		fill[i + 32] = F_REGISTERS[i];
	fill[64] = PC;
	env->SetIntArrayRegion(result, (jsize) 0, (jsize) 65, (const jint*) fill);
	return result;
}

/*
 * Class:     Debugger
 * Method:    setRegisters
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Debugger_setRegisters(JNIEnv *env, jobject obj,
		jintArray registers){
	jint *body = env->GetIntArrayElements(registers, 0);
	for(int i = 0 ; i < 32 ; i++){
		REGISTERS[i] = (uint32_t)body[i];
		F_REGISTERS[i] = (uint32_t)body[i+32];
	}
	PC = body[64];
	env->ReleaseIntArrayElements(registers, body, 0);
}

/*
 * Class:     Debugger
 * Method:    getMemorySegment
 * Signature: ()[C
 */
JNIEXPORT void JNICALL Java_Debugger_getMemorySegment(JNIEnv *, jobject) {
	FILE* file = fopen(MEMORY_FILE_NAME, "w");
	fwrite(MEMORY_SEGMENT, 1, MEMORY_SIZE * 1024, file);
	fclose(file);
}

/*
 * Class:     Debugger
 * Method:    getMemoryOffset
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Debugger_getMemoryOffset(JNIEnv *env, jobject obj) {
	return (jint) MEMORY_OFFSET;
}

/*
 * Class:     Debugger
 * Method:    getMemorySizeBytes
 * Signature: ()L
 */
JNIEXPORT jlong JNICALL Java_Debugger_getMemorySizeBytes
  (JNIEnv *env, jobject obj){
	return MEMORY_SIZE*1024;
}

/*
 * Class:     Debugger
 * Method:    getCodeOffset
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Debugger_getCodeOffset(JNIEnv *, jobject) {
	return (jint) CODE_OFFSET;
}

