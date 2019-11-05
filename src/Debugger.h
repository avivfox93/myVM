/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class Debugger */

#ifndef _Included_Debugger
#define _Included_Debugger
#ifdef __cplusplus
extern "C" {
#endif

#define READ	0
#define WRITE	1

JNIEXPORT jboolean JNICALL Java_Debugger_begin(JNIEnv * , jobject ,
		jstring, jstring);

/*
 * Class:     Debugger
 * Method:    decompileFile
 * Signature: ()V
 */
JNIEXPORT jstring JNICALL Java_Debugger_decompileFile(JNIEnv * , jobject );

/*
 * Class:     Debugger
 * Method:    updateMemory
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Debugger_updateMemory(JNIEnv * env, jobject obj);

/*
 * Class:     Debugger
 * Method:    loadFile
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT int JNICALL Java_Debugger_loadFile
  (JNIEnv *, jobject, jstring);

/*
 * Class:     Debugger
 * Method:    runNext
 * Signature: ()V
 */
JNIEXPORT jstring JNICALL Java_Debugger_runNext
  (JNIEnv *, jobject);

/*
 * Class:     Debugger
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Debugger_stop
  (JNIEnv *, jobject);

/*
 * Class:     Debugger
 * Method:    getRegisters
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_Debugger_getRegisters
  (JNIEnv *, jobject);

/*
 * Class:     Debugger
 * Method:    setRegisters
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_Debugger_setRegisters
  (JNIEnv *, jobject, jintArray);

/*
 * Class:     Debugger
 * Method:    getMemorySegment
 * Signature: ()[C
 */
JNIEXPORT void JNICALL Java_Debugger_getMemorySegment
  (JNIEnv *, jobject);

/*
 * Class:     Debugger
 * Method:    getMemoryOffset
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Debugger_getMemoryOffset
  (JNIEnv *, jobject);

/*
 * Class:     Debugger
 * Method:    getMemorySizeBytes
 * Signature: ()L
 */
JNIEXPORT jlong JNICALL Java_Debugger_getMemorySizeBytes
  (JNIEnv *, jobject);

/*
 * Class:     Debugger
 * Method:    getCodeOffset
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_Debugger_getCodeOffset
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
