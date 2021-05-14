// Copyright 2016 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Disclaimer: This is not an official Google product.

#ifndef JNI_BIND_MOCK_JNI_ENV_H_
#define JNI_BIND_MOCK_JNI_ENV_H_

#include <gmock/gmock.h>
#include "jni_dep.h"

namespace jni::test {

#if defined(__ANDROID__) && __NDK_MAJOR__ < 18
using InputJvalueArray = jvalue*;
#else
using InputJvalueArray = const jvalue*;
#endif

// This is the complete version of MockJNIEnv. Since there are about 200 virtual
// functions, the gmock object is too large to put on stack because the default
// stack frame size limit is 16k. You can either add "-Wframe-larger-than=32768"
// to copts of your test, or create the object from heap.
//
// Note that it also takes a long time to compile this class. If you want to
// have a faster test compilation time, consider moving the functions you need
// to MockJNIEnv.
class MockableJNIEnvFull : public JNIEnv {
 public:
  MockableJNIEnvFull() {
    functions = &functions_;
    functions_.AllocObject = &CallAllocObject;
    functions_.CallBooleanMethodA = &CallCallBooleanMethodA;
    functions_.CallBooleanMethodV = &CallCallBooleanMethodV;
    functions_.CallByteMethodA = &CallCallByteMethodA;
    functions_.CallByteMethodV = &CallCallByteMethodV;
    functions_.CallCharMethodA = &CallCallCharMethodA;
    functions_.CallCharMethodV = &CallCallCharMethodV;
    functions_.CallDoubleMethodA = &CallCallDoubleMethodA;
    functions_.CallDoubleMethodV = &CallCallDoubleMethodV;
    functions_.CallFloatMethodA = &CallCallFloatMethodA;
    functions_.CallFloatMethodV = &CallCallFloatMethodV;
    functions_.CallIntMethodA = &CallCallIntMethodA;
    functions_.CallIntMethodV = &CallCallIntMethodV;
    functions_.CallLongMethodA = &CallCallLongMethodA;
    functions_.CallLongMethodV = &CallCallLongMethodV;
    functions_.CallNonvirtualBooleanMethodA = &CallCallNonvirtualBooleanMethodA;
    functions_.CallNonvirtualBooleanMethodV = &CallCallNonvirtualBooleanMethodV;
    functions_.CallNonvirtualByteMethodA = &CallCallNonvirtualByteMethodA;
    functions_.CallNonvirtualByteMethodV = &CallCallNonvirtualByteMethodV;
    functions_.CallNonvirtualCharMethodA = &CallCallNonvirtualCharMethodA;
    functions_.CallNonvirtualCharMethodV = &CallCallNonvirtualCharMethodV;
    functions_.CallNonvirtualDoubleMethodA = &CallCallNonvirtualDoubleMethodA;
    functions_.CallNonvirtualDoubleMethodV = &CallCallNonvirtualDoubleMethodV;
    functions_.CallNonvirtualFloatMethodA = &CallCallNonvirtualFloatMethodA;
    functions_.CallNonvirtualFloatMethodV = &CallCallNonvirtualFloatMethodV;
    functions_.CallNonvirtualIntMethodA = &CallCallNonvirtualIntMethodA;
    functions_.CallNonvirtualIntMethodV = &CallCallNonvirtualIntMethodV;
    functions_.CallNonvirtualLongMethodA = &CallCallNonvirtualLongMethodA;
    functions_.CallNonvirtualLongMethodV = &CallCallNonvirtualLongMethodV;
    functions_.CallNonvirtualObjectMethodA = &CallCallNonvirtualObjectMethodA;
    functions_.CallNonvirtualObjectMethodV = &CallCallNonvirtualObjectMethodV;
    functions_.CallNonvirtualShortMethodA = &CallCallNonvirtualShortMethodA;
    functions_.CallNonvirtualShortMethodV = &CallCallNonvirtualShortMethodV;
    functions_.CallNonvirtualVoidMethodA = &CallCallNonvirtualVoidMethodA;
    functions_.CallNonvirtualVoidMethodV = &CallCallNonvirtualVoidMethodV;
    functions_.CallObjectMethodA = &CallCallObjectMethodA;
    functions_.CallObjectMethodV = &CallCallObjectMethodV;
    functions_.CallShortMethodA = &CallCallShortMethodA;
    functions_.CallShortMethodV = &CallCallShortMethodV;
    functions_.CallStaticBooleanMethodA = &CallCallStaticBooleanMethodA;
    functions_.CallStaticBooleanMethodV = &CallCallStaticBooleanMethodV;
    functions_.CallStaticByteMethodA = &CallCallStaticByteMethodA;
    functions_.CallStaticByteMethodV = &CallCallStaticByteMethodV;
    functions_.CallStaticCharMethodA = &CallCallStaticCharMethodA;
    functions_.CallStaticCharMethodV = &CallCallStaticCharMethodV;
    functions_.CallStaticDoubleMethodA = &CallCallStaticDoubleMethodA;
    functions_.CallStaticDoubleMethodV = &CallCallStaticDoubleMethodV;
    functions_.CallStaticFloatMethodA = &CallCallStaticFloatMethodA;
    functions_.CallStaticFloatMethodV = &CallCallStaticFloatMethodV;
    functions_.CallStaticIntMethodA = &CallCallStaticIntMethodA;
    functions_.CallStaticIntMethodV = &CallCallStaticIntMethodV;
    functions_.CallStaticLongMethodA = &CallCallStaticLongMethodA;
    functions_.CallStaticLongMethodV = &CallCallStaticLongMethodV;
    functions_.CallStaticObjectMethodA = &CallCallStaticObjectMethodA;
    functions_.CallStaticObjectMethodV = &CallCallStaticObjectMethodV;
    functions_.CallStaticShortMethodA = &CallCallStaticShortMethodA;
    functions_.CallStaticShortMethodV = &CallCallStaticShortMethodV;
    functions_.CallStaticVoidMethodA = &CallCallStaticVoidMethodA;
    functions_.CallStaticVoidMethodV = &CallCallStaticVoidMethodV;
    functions_.CallVoidMethodA = &CallCallVoidMethodA;
    functions_.CallVoidMethodV = &CallCallVoidMethodV;
    functions_.DefineClass = &CallDefineClass;
    functions_.DeleteGlobalRef = &CallDeleteGlobalRef;
    functions_.DeleteLocalRef = &CallDeleteLocalRef;
    functions_.DeleteWeakGlobalRef = &CallDeleteWeakGlobalRef;
    functions_.EnsureLocalCapacity = &CallEnsureLocalCapacity;
    functions_.ExceptionCheck = &CallExceptionCheck;
    functions_.ExceptionClear = &CallExceptionClear;
    functions_.ExceptionDescribe = &CallExceptionDescribe;
    functions_.ExceptionOccurred = &CallExceptionOccurred;
    functions_.FatalError = &CallFatalError;
    functions_.FindClass = &CallFindClass;
    functions_.FromReflectedField = &CallFromReflectedField;
    functions_.FromReflectedMethod = &CallFromReflectedMethod;
    functions_.GetArrayLength = &CallGetArrayLength;
    functions_.GetBooleanArrayElements = &CallGetBooleanArrayElements;
    functions_.GetBooleanArrayRegion = &CallGetBooleanArrayRegion;
    functions_.GetBooleanField = &CallGetBooleanField;
    functions_.GetByteArrayElements = &CallGetByteArrayElements;
    functions_.GetByteArrayRegion = &CallGetByteArrayRegion;
    functions_.GetByteField = &CallGetByteField;
    functions_.GetCharArrayElements = &CallGetCharArrayElements;
    functions_.GetCharArrayRegion = &CallGetCharArrayRegion;
    functions_.GetCharField = &CallGetCharField;
    functions_.GetDirectBufferAddress = &CallGetDirectBufferAddress;
    functions_.GetDirectBufferCapacity = &CallGetDirectBufferCapacity;
    functions_.GetDoubleArrayElements = &CallGetDoubleArrayElements;
    functions_.GetDoubleArrayRegion = &CallGetDoubleArrayRegion;
    functions_.GetDoubleField = &CallGetDoubleField;
    functions_.GetFieldID = &CallGetFieldID;
    functions_.GetFloatArrayElements = &CallGetFloatArrayElements;
    functions_.GetFloatArrayRegion = &CallGetFloatArrayRegion;
    functions_.GetFloatField = &CallGetFloatField;
    functions_.GetIntArrayElements = &CallGetIntArrayElements;
    functions_.GetIntArrayRegion = &CallGetIntArrayRegion;
    functions_.GetIntField = &CallGetIntField;
    functions_.GetJavaVM = &CallGetJavaVM;
    functions_.GetLongArrayElements = &CallGetLongArrayElements;
    functions_.GetLongArrayRegion = &CallGetLongArrayRegion;
    functions_.GetLongField = &CallGetLongField;
    functions_.GetMethodID = &CallGetMethodID;
    functions_.GetObjectArrayElement = &CallGetObjectArrayElement;
    functions_.GetObjectClass = &CallGetObjectClass;
    functions_.GetObjectField = &CallGetObjectField;
    functions_.GetObjectRefType = &CallGetObjectRefType;
    functions_.GetPrimitiveArrayCritical = &CallGetPrimitiveArrayCritical;
    functions_.GetShortArrayElements = &CallGetShortArrayElements;
    functions_.GetShortArrayRegion = &CallGetShortArrayRegion;
    functions_.GetShortField = &CallGetShortField;
    functions_.GetStaticBooleanField = &CallGetStaticBooleanField;
    functions_.GetStaticByteField = &CallGetStaticByteField;
    functions_.GetStaticCharField = &CallGetStaticCharField;
    functions_.GetStaticDoubleField = &CallGetStaticDoubleField;
    functions_.GetStaticFieldID = &CallGetStaticFieldID;
    functions_.GetStaticFloatField = &CallGetStaticFloatField;
    functions_.GetStaticIntField = &CallGetStaticIntField;
    functions_.GetStaticLongField = &CallGetStaticLongField;
    functions_.GetStaticMethodID = &CallGetStaticMethodID;
    functions_.GetStaticObjectField = &CallGetStaticObjectField;
    functions_.GetStaticShortField = &CallGetStaticShortField;
    functions_.GetStringChars = &CallGetStringChars;
    functions_.GetStringCritical = &CallGetStringCritical;
    functions_.GetStringLength = &CallGetStringLength;
    functions_.GetStringRegion = &CallGetStringRegion;
    functions_.GetStringUTFChars = &CallGetStringUTFChars;
    functions_.GetStringUTFLength = &CallGetStringUTFLength;
    functions_.GetStringUTFRegion = &CallGetStringUTFRegion;
    functions_.GetSuperclass = &CallGetSuperclass;
    functions_.GetVersion = &CallGetVersion;
    functions_.IsAssignableFrom = &CallIsAssignableFrom;
    functions_.IsInstanceOf = &CallIsInstanceOf;
    functions_.IsSameObject = &CallIsSameObject;
    functions_.MonitorEnter = &CallMonitorEnter;
    functions_.MonitorExit = &CallMonitorExit;
    functions_.NewBooleanArray = &CallNewBooleanArray;
    functions_.NewByteArray = &CallNewByteArray;
    functions_.NewCharArray = &CallNewCharArray;
    functions_.NewDirectByteBuffer = &CallNewDirectByteBuffer;
    functions_.NewDoubleArray = &CallNewDoubleArray;
    functions_.NewFloatArray = &CallNewFloatArray;
    functions_.NewGlobalRef = &CallNewGlobalRef;
    functions_.NewIntArray = &CallNewIntArray;
    functions_.NewLocalRef = &CallNewLocalRef;
    functions_.NewLongArray = &CallNewLongArray;
    functions_.NewObjectA = &CallNewObjectA;
    functions_.NewObjectArray = &CallNewObjectArray;
    functions_.NewObjectV = &CallNewObjectV;
    functions_.NewShortArray = &CallNewShortArray;
    functions_.NewString = &CallNewString;
    functions_.NewStringUTF = &CallNewStringUTF;
    functions_.NewWeakGlobalRef = &CallNewWeakGlobalRef;
    functions_.PopLocalFrame = &CallPopLocalFrame;
    functions_.PushLocalFrame = &CallPushLocalFrame;
    functions_.RegisterNatives = &CallRegisterNatives;
    functions_.ReleaseBooleanArrayElements = &CallReleaseBooleanArrayElements;
    functions_.ReleaseByteArrayElements = &CallReleaseByteArrayElements;
    functions_.ReleaseCharArrayElements = &CallReleaseCharArrayElements;
    functions_.ReleaseDoubleArrayElements = &CallReleaseDoubleArrayElements;
    functions_.ReleaseFloatArrayElements = &CallReleaseFloatArrayElements;
    functions_.ReleaseIntArrayElements = &CallReleaseIntArrayElements;
    functions_.ReleaseLongArrayElements = &CallReleaseLongArrayElements;
    functions_.ReleasePrimitiveArrayCritical =
        &CallReleasePrimitiveArrayCritical;
    functions_.ReleaseShortArrayElements = &CallReleaseShortArrayElements;
    functions_.ReleaseStringChars = &CallReleaseStringChars;
    functions_.ReleaseStringCritical = &CallReleaseStringCritical;
    functions_.ReleaseStringUTFChars = &CallReleaseStringUTFChars;
    functions_.SetBooleanArrayRegion = &CallSetBooleanArrayRegion;
    functions_.SetBooleanField = &CallSetBooleanField;
    functions_.SetByteArrayRegion = &CallSetByteArrayRegion;
    functions_.SetByteField = &CallSetByteField;
    functions_.SetCharArrayRegion = &CallSetCharArrayRegion;
    functions_.SetCharField = &CallSetCharField;
    functions_.SetDoubleArrayRegion = &CallSetDoubleArrayRegion;
    functions_.SetDoubleField = &CallSetDoubleField;
    functions_.SetFloatArrayRegion = &CallSetFloatArrayRegion;
    functions_.SetFloatField = &CallSetFloatField;
    functions_.SetIntArrayRegion = &CallSetIntArrayRegion;
    functions_.SetIntField = &CallSetIntField;
    functions_.SetLongArrayRegion = &CallSetLongArrayRegion;
    functions_.SetLongField = &CallSetLongField;
    functions_.SetObjectArrayElement = &CallSetObjectArrayElement;
    functions_.SetObjectField = &CallSetObjectField;
    functions_.SetShortArrayRegion = &CallSetShortArrayRegion;
    functions_.SetShortField = &CallSetShortField;
    functions_.SetStaticBooleanField = &CallSetStaticBooleanField;
    functions_.SetStaticByteField = &CallSetStaticByteField;
    functions_.SetStaticCharField = &CallSetStaticCharField;
    functions_.SetStaticDoubleField = &CallSetStaticDoubleField;
    functions_.SetStaticFloatField = &CallSetStaticFloatField;
    functions_.SetStaticIntField = &CallSetStaticIntField;
    functions_.SetStaticLongField = &CallSetStaticLongField;
    functions_.SetStaticObjectField = &CallSetStaticObjectField;
    functions_.SetStaticShortField = &CallSetStaticShortField;
    functions_.Throw = &CallThrow;
    functions_.ThrowNew = &CallThrowNew;
    functions_.ToReflectedField = &CallToReflectedField;
    functions_.ToReflectedMethod = &CallToReflectedMethod;
    functions_.UnregisterNatives = &CallUnregisterNatives;
  }
  virtual ~MockableJNIEnvFull() {}

  virtual jobject AllocObject(jclass clazz) = 0;
  virtual jboolean CallBooleanMethodA(jobject obj, jmethodID methodID,
                                      InputJvalueArray args) = 0;
  virtual jboolean CallBooleanMethodV(jobject obj, jmethodID methodID,
                                      va_list args) = 0;
  virtual jbyte CallByteMethodA(jobject obj, jmethodID methodID,
                                InputJvalueArray args) = 0;
  virtual jbyte CallByteMethodV(jobject obj, jmethodID methodID,
                                va_list args) = 0;
  virtual jchar CallCharMethodA(jobject obj, jmethodID methodID,
                                InputJvalueArray args) = 0;
  virtual jchar CallCharMethodV(jobject obj, jmethodID methodID,
                                va_list args) = 0;
  virtual jdouble CallDoubleMethodA(jobject obj, jmethodID methodID,
                                    InputJvalueArray args) = 0;
  virtual jdouble CallDoubleMethodV(jobject obj, jmethodID methodID,
                                    va_list args) = 0;
  virtual jfloat CallFloatMethodA(jobject obj, jmethodID methodID,
                                  InputJvalueArray args) = 0;
  virtual jfloat CallFloatMethodV(jobject obj, jmethodID methodID,
                                  va_list args) = 0;
  virtual jint CallIntMethodA(jobject obj, jmethodID methodID,
                              InputJvalueArray args) = 0;
  virtual jint CallIntMethodV(jobject obj, jmethodID methodID,
                              va_list args) = 0;
  virtual jlong CallLongMethodA(jobject obj, jmethodID methodID,
                                InputJvalueArray args) = 0;
  virtual jlong CallLongMethodV(jobject obj, jmethodID methodID,
                                va_list args) = 0;
  virtual jboolean CallNonvirtualBooleanMethodA(jobject obj, jclass clazz,
                                                jmethodID methodID,
                                                InputJvalueArray args) = 0;
  virtual jboolean CallNonvirtualBooleanMethodV(jobject obj, jclass clazz,
                                                jmethodID methodID,
                                                va_list args) = 0;
  virtual jbyte CallNonvirtualByteMethodA(jobject obj, jclass clazz,
                                          jmethodID methodID,
                                          InputJvalueArray args) = 0;
  virtual jbyte CallNonvirtualByteMethodV(jobject obj, jclass clazz,
                                          jmethodID methodID, va_list args) = 0;
  virtual jchar CallNonvirtualCharMethodA(jobject obj, jclass clazz,
                                          jmethodID methodID,
                                          InputJvalueArray args) = 0;
  virtual jchar CallNonvirtualCharMethodV(jobject obj, jclass clazz,
                                          jmethodID methodID, va_list args) = 0;
  virtual jdouble CallNonvirtualDoubleMethodA(jobject obj, jclass clazz,
                                              jmethodID methodID,
                                              InputJvalueArray args) = 0;
  virtual jdouble CallNonvirtualDoubleMethodV(jobject obj, jclass clazz,
                                              jmethodID methodID,
                                              va_list args) = 0;
  virtual jfloat CallNonvirtualFloatMethodA(jobject obj, jclass clazz,
                                            jmethodID methodID,
                                            InputJvalueArray args) = 0;
  virtual jfloat CallNonvirtualFloatMethodV(jobject obj, jclass clazz,
                                            jmethodID methodID,
                                            va_list args) = 0;
  virtual jint CallNonvirtualIntMethodA(jobject obj, jclass clazz,
                                        jmethodID methodID,
                                        InputJvalueArray args) = 0;
  virtual jint CallNonvirtualIntMethodV(jobject obj, jclass clazz,
                                        jmethodID methodID, va_list args) = 0;
  virtual jlong CallNonvirtualLongMethodA(jobject obj, jclass clazz,
                                          jmethodID methodID,
                                          InputJvalueArray args) = 0;
  virtual jlong CallNonvirtualLongMethodV(jobject obj, jclass clazz,
                                          jmethodID methodID, va_list args) = 0;
  virtual jobject CallNonvirtualObjectMethodA(jobject obj, jclass clazz,
                                              jmethodID methodID,
                                              InputJvalueArray args) = 0;
  virtual jobject CallNonvirtualObjectMethodV(jobject obj, jclass clazz,
                                              jmethodID methodID,
                                              va_list args) = 0;
  virtual jshort CallNonvirtualShortMethodA(jobject obj, jclass clazz,
                                            jmethodID methodID,
                                            InputJvalueArray args) = 0;
  virtual jshort CallNonvirtualShortMethodV(jobject obj, jclass clazz,
                                            jmethodID methodID,
                                            va_list args) = 0;
  virtual void CallNonvirtualVoidMethodA(jobject obj, jclass clazz,
                                         jmethodID methodID,
                                         InputJvalueArray args) = 0;
  virtual void CallNonvirtualVoidMethodV(jobject obj, jclass clazz,
                                         jmethodID methodID, va_list args) = 0;
  virtual jobject CallObjectMethodA(jobject obj, jmethodID methodID,
                                    InputJvalueArray args) = 0;
  virtual jobject CallObjectMethodV(jobject obj, jmethodID methodID,
                                    va_list args) = 0;
  virtual jshort CallShortMethodA(jobject obj, jmethodID methodID,
                                  InputJvalueArray args) = 0;
  virtual jshort CallShortMethodV(jobject obj, jmethodID methodID,
                                  va_list args) = 0;
  virtual jboolean CallStaticBooleanMethodA(jclass clazz, jmethodID methodID,
                                            InputJvalueArray args) = 0;
  virtual jboolean CallStaticBooleanMethodV(jclass clazz, jmethodID methodID,
                                            va_list args) = 0;
  virtual jbyte CallStaticByteMethodA(jclass clazz, jmethodID methodID,
                                      InputJvalueArray args) = 0;
  virtual jbyte CallStaticByteMethodV(jclass clazz, jmethodID methodID,
                                      va_list args) = 0;
  virtual jchar CallStaticCharMethodA(jclass clazz, jmethodID methodID,
                                      InputJvalueArray args) = 0;
  virtual jchar CallStaticCharMethodV(jclass clazz, jmethodID methodID,
                                      va_list args) = 0;
  virtual jdouble CallStaticDoubleMethodA(jclass clazz, jmethodID methodID,
                                          InputJvalueArray args) = 0;
  virtual jdouble CallStaticDoubleMethodV(jclass clazz, jmethodID methodID,
                                          va_list args) = 0;
  virtual jfloat CallStaticFloatMethodA(jclass clazz, jmethodID methodID,
                                        InputJvalueArray args) = 0;
  virtual jfloat CallStaticFloatMethodV(jclass clazz, jmethodID methodID,
                                        va_list args) = 0;
  virtual jint CallStaticIntMethodA(jclass clazz, jmethodID methodID,
                                    InputJvalueArray args) = 0;
  virtual jint CallStaticIntMethodV(jclass clazz, jmethodID methodID,
                                    va_list args) = 0;
  virtual jlong CallStaticLongMethodA(jclass clazz, jmethodID methodID,
                                      InputJvalueArray args) = 0;
  virtual jlong CallStaticLongMethodV(jclass clazz, jmethodID methodID,
                                      va_list args) = 0;
  virtual jobject CallStaticObjectMethodA(jclass clazz, jmethodID methodID,
                                          InputJvalueArray args) = 0;
  virtual jobject CallStaticObjectMethodV(jclass clazz, jmethodID methodID,
                                          va_list args) = 0;
  virtual jshort CallStaticShortMethodA(jclass clazz, jmethodID methodID,
                                        InputJvalueArray args) = 0;
  virtual jshort CallStaticShortMethodV(jclass clazz, jmethodID methodID,
                                        va_list args) = 0;
  virtual void CallStaticVoidMethodA(jclass cls, jmethodID methodID,
                                     InputJvalueArray args) = 0;
  virtual void CallStaticVoidMethodV(jclass cls, jmethodID methodID,
                                     va_list args) = 0;
  virtual void CallVoidMethodA(jobject obj, jmethodID methodID,
                               InputJvalueArray args) = 0;
  virtual void CallVoidMethodV(jobject obj, jmethodID methodID,
                               va_list args) = 0;
  virtual jclass DefineClass(const char* name, jobject loader, const jbyte* buf,
                             jsize len) = 0;
  virtual void DeleteGlobalRef(jobject gref) = 0;
  virtual void DeleteLocalRef(jobject obj) = 0;
  virtual void DeleteWeakGlobalRef(jweak ref) = 0;
  virtual jint EnsureLocalCapacity(jint capacity) = 0;
  virtual jboolean ExceptionCheck() = 0;
  virtual void ExceptionClear() = 0;
  virtual void ExceptionDescribe() = 0;
  virtual jthrowable ExceptionOccurred() = 0;
  virtual void FatalError(const char* msg) = 0;
  virtual jclass FindClass(const char* name) = 0;
  virtual jfieldID FromReflectedField(jobject field) = 0;
  virtual jmethodID FromReflectedMethod(jobject method) = 0;
  virtual jsize GetArrayLength(jarray array) = 0;
  virtual jboolean* GetBooleanArrayElements(jbooleanArray array,
                                            jboolean* isCopy) = 0;
  virtual void GetBooleanArrayRegion(jbooleanArray array, jsize start,
                                     jsize len, jboolean* buf) = 0;
  virtual jboolean GetBooleanField(jobject obj, jfieldID fieldID) = 0;
  virtual jbyte* GetByteArrayElements(jbyteArray array, jboolean* isCopy) = 0;
  virtual void GetByteArrayRegion(jbyteArray array, jsize start, jsize len,
                                  jbyte* buf) = 0;
  virtual jbyte GetByteField(jobject obj, jfieldID fieldID) = 0;
  virtual jchar* GetCharArrayElements(jcharArray array, jboolean* isCopy) = 0;
  virtual void GetCharArrayRegion(jcharArray array, jsize start, jsize len,
                                  jchar* buf) = 0;
  virtual jchar GetCharField(jobject obj, jfieldID fieldID) = 0;
  virtual void* GetDirectBufferAddress(jobject buf) = 0;
  virtual jlong GetDirectBufferCapacity(jobject buf) = 0;
  virtual jdouble* GetDoubleArrayElements(jdoubleArray array,
                                          jboolean* isCopy) = 0;
  virtual void GetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len,
                                    jdouble* buf) = 0;
  virtual jdouble GetDoubleField(jobject obj, jfieldID fieldID) = 0;
  virtual jfieldID GetFieldID(jclass clazz, const char* name,
                              const char* sig) = 0;
  virtual jfloat* GetFloatArrayElements(jfloatArray array,
                                        jboolean* isCopy) = 0;
  virtual void GetFloatArrayRegion(jfloatArray array, jsize start, jsize len,
                                   jfloat* buf) = 0;
  virtual jfloat GetFloatField(jobject obj, jfieldID fieldID) = 0;
  virtual jint* GetIntArrayElements(jintArray array, jboolean* isCopy) = 0;
  virtual void GetIntArrayRegion(jintArray array, jsize start, jsize len,
                                 jint* buf) = 0;
  virtual jint GetIntField(jobject obj, jfieldID fieldID) = 0;
  virtual jint GetJavaVM(JavaVM** vm) = 0;
  virtual jlong* GetLongArrayElements(jlongArray array, jboolean* isCopy) = 0;
  virtual void GetLongArrayRegion(jlongArray array, jsize start, jsize len,
                                  jlong* buf) = 0;
  virtual jlong GetLongField(jobject obj, jfieldID fieldID) = 0;
  virtual jmethodID GetMethodID(jclass clazz, const char* name,
                                const char* sig) = 0;
  virtual jobject GetObjectArrayElement(jobjectArray array, jsize index) = 0;
  virtual jclass GetObjectClass(jobject obj) = 0;
  virtual jobject GetObjectField(jobject obj, jfieldID fieldID) = 0;
  virtual jobjectRefType GetObjectRefType(jobject obj) = 0;
  virtual void* GetPrimitiveArrayCritical(jarray array, jboolean* isCopy) = 0;
  virtual jshort* GetShortArrayElements(jshortArray array,
                                        jboolean* isCopy) = 0;
  virtual void GetShortArrayRegion(jshortArray array, jsize start, jsize len,
                                   jshort* buf) = 0;
  virtual jshort GetShortField(jobject obj, jfieldID fieldID) = 0;
  virtual jboolean GetStaticBooleanField(jclass clazz, jfieldID fieldID) = 0;
  virtual jbyte GetStaticByteField(jclass clazz, jfieldID fieldID) = 0;
  virtual jchar GetStaticCharField(jclass clazz, jfieldID fieldID) = 0;
  virtual jdouble GetStaticDoubleField(jclass clazz, jfieldID fieldID) = 0;
  virtual jfieldID GetStaticFieldID(jclass clazz, const char* name,
                                    const char* sig) = 0;
  virtual jfloat GetStaticFloatField(jclass clazz, jfieldID fieldID) = 0;
  virtual jint GetStaticIntField(jclass clazz, jfieldID fieldID) = 0;
  virtual jlong GetStaticLongField(jclass clazz, jfieldID fieldID) = 0;
  virtual jmethodID GetStaticMethodID(jclass clazz, const char* name,
                                      const char* sig) = 0;
  virtual jobject GetStaticObjectField(jclass clazz, jfieldID fieldID) = 0;
  virtual jshort GetStaticShortField(jclass clazz, jfieldID fieldID) = 0;
  virtual jsize GetStringLength(jstring str) = 0;
  virtual void GetStringRegion(jstring str, jsize start, jsize len,
                               jchar* buf) = 0;
  virtual const char* GetStringUTFChars(jstring str, jboolean* isCopy) = 0;
  virtual jsize GetStringUTFLength(jstring str) = 0;
  virtual void GetStringUTFRegion(jstring str, jsize start, jsize len,
                                  char* buf) = 0;
  virtual jclass GetSuperclass(jclass sub) = 0;
  virtual jint GetVersion() = 0;
  virtual jboolean IsAssignableFrom(jclass sub, jclass sup) = 0;
  virtual jboolean IsInstanceOf(jobject obj, jclass clazz) = 0;
  virtual jboolean IsSameObject(jobject obj1, jobject obj2) = 0;
  virtual const jchar* GetStringChars(jstring str, jboolean* isCopy) = 0;
  virtual const jchar* GetStringCritical(jstring str, jboolean* isCopy) = 0;
  virtual jint MonitorEnter(jobject obj) = 0;
  virtual jint MonitorExit(jobject obj) = 0;
  virtual jbooleanArray NewBooleanArray(jsize len) = 0;
  virtual jbyteArray NewByteArray(jsize len) = 0;
  virtual jcharArray NewCharArray(jsize len) = 0;
  virtual jobject NewDirectByteBuffer(void* address, jlong capacity) = 0;
  virtual jdoubleArray NewDoubleArray(jsize len) = 0;
  virtual jfloatArray NewFloatArray(jsize len) = 0;
  virtual jobject NewGlobalRef(jobject lobj) = 0;
  virtual jintArray NewIntArray(jsize len) = 0;
  virtual jobject NewLocalRef(jobject ref) = 0;
  virtual jlongArray NewLongArray(jsize len) = 0;
  virtual jobject NewObjectA(jclass clazz, jmethodID methodID,
                             InputJvalueArray args) = 0;
  virtual jobjectArray NewObjectArray(jsize len, jclass clazz,
                                      jobject init) = 0;
  virtual jobject NewObjectV(jclass clazz, jmethodID methodID,
                             va_list args) = 0;
  virtual jshortArray NewShortArray(jsize len) = 0;
  virtual jstring NewString(const jchar* unicode, jsize len) = 0;
  virtual jstring NewStringUTF(const char* utf) = 0;
  virtual jweak NewWeakGlobalRef(jobject obj) = 0;
  virtual jobject PopLocalFrame(jobject result) = 0;
  virtual jint PushLocalFrame(jint capacity) = 0;
  virtual jint RegisterNatives(jclass clazz, const JNINativeMethod* methods,
                               jint nMethods) = 0;
  virtual void ReleaseBooleanArrayElements(jbooleanArray array, jboolean* elems,
                                           jint mode) = 0;
  virtual void ReleaseByteArrayElements(jbyteArray array, jbyte* elems,
                                        jint mode) = 0;
  virtual void ReleaseCharArrayElements(jcharArray array, jchar* elems,
                                        jint mode) = 0;
  virtual void ReleaseDoubleArrayElements(jdoubleArray array, jdouble* elems,
                                          jint mode) = 0;
  virtual void ReleaseFloatArrayElements(jfloatArray array, jfloat* elems,
                                         jint mode) = 0;
  virtual void ReleaseIntArrayElements(jintArray array, jint* elems,
                                       jint mode) = 0;
  virtual void ReleaseLongArrayElements(jlongArray array, jlong* elems,
                                        jint mode) = 0;
  virtual void ReleasePrimitiveArrayCritical(jarray array, void* carray,
                                             jint mode) = 0;
  virtual void ReleaseShortArrayElements(jshortArray array, jshort* elems,
                                         jint mode) = 0;
  virtual void ReleaseStringChars(jstring str, const jchar* chars) = 0;
  virtual void ReleaseStringCritical(jstring str, const jchar* cstring) = 0;
  virtual void ReleaseStringUTFChars(jstring str, const char* chars) = 0;
  virtual void SetBooleanArrayRegion(jbooleanArray array, jsize start,
                                     jsize len, const jboolean* buf) = 0;
  virtual void SetBooleanField(jobject obj, jfieldID fieldID, jboolean val) = 0;
  virtual void SetByteArrayRegion(jbyteArray array, jsize start, jsize len,
                                  const jbyte* buf) = 0;
  virtual void SetByteField(jobject obj, jfieldID fieldID, jbyte val) = 0;
  virtual void SetCharArrayRegion(jcharArray array, jsize start, jsize len,
                                  const jchar* buf) = 0;
  virtual void SetCharField(jobject obj, jfieldID fieldID, jchar val) = 0;
  virtual void SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len,
                                    const jdouble* buf) = 0;
  virtual void SetDoubleField(jobject obj, jfieldID fieldID, jdouble val) = 0;
  virtual void SetFloatArrayRegion(jfloatArray array, jsize start, jsize len,
                                   const jfloat* buf) = 0;
  virtual void SetFloatField(jobject obj, jfieldID fieldID, jfloat val) = 0;
  virtual void SetIntArrayRegion(jintArray array, jsize start, jsize len,
                                 const jint* buf) = 0;
  virtual void SetIntField(jobject obj, jfieldID fieldID, jint val) = 0;
  virtual void SetLongArrayRegion(jlongArray array, jsize start, jsize len,
                                  const jlong* buf) = 0;
  virtual void SetLongField(jobject obj, jfieldID fieldID, jlong val) = 0;
  virtual void SetObjectArrayElement(jobjectArray array, jsize index,
                                     jobject val) = 0;
  virtual void SetObjectField(jobject obj, jfieldID fieldID, jobject val) = 0;
  virtual void SetShortArrayRegion(jshortArray array, jsize start, jsize len,
                                   const jshort* buf) = 0;
  virtual void SetShortField(jobject obj, jfieldID fieldID, jshort val) = 0;
  virtual void SetStaticBooleanField(jclass clazz, jfieldID fieldID,
                                     jboolean value) = 0;
  virtual void SetStaticByteField(jclass clazz, jfieldID fieldID,
                                  jbyte value) = 0;
  virtual void SetStaticCharField(jclass clazz, jfieldID fieldID,
                                  jchar value) = 0;
  virtual void SetStaticDoubleField(jclass clazz, jfieldID fieldID,
                                    jdouble value) = 0;
  virtual void SetStaticFloatField(jclass clazz, jfieldID fieldID,
                                   jfloat value) = 0;
  virtual void SetStaticIntField(jclass clazz, jfieldID fieldID,
                                 jint value) = 0;
  virtual void SetStaticLongField(jclass clazz, jfieldID fieldID,
                                  jlong value) = 0;
  virtual void SetStaticObjectField(jclass clazz, jfieldID fieldID,
                                    jobject value) = 0;
  virtual void SetStaticShortField(jclass clazz, jfieldID fieldID,
                                   jshort value) = 0;
  virtual jint Throw(jthrowable obj) = 0;
  virtual jint ThrowNew(jclass clazz, const char* msg) = 0;
  virtual jobject ToReflectedField(jclass cls, jfieldID fieldID,
                                   jboolean isStatic) = 0;
  virtual jobject ToReflectedMethod(jclass cls, jmethodID methodID,
                                    jboolean isStatic) = 0;
  virtual jint UnregisterNatives(jclass clazz) = 0;

 private:
  static jobject JNICALL CallAllocObject(JNIEnv* env, jclass clazz) {
    return static_cast<MockableJNIEnvFull*>(env)->AllocObject(clazz);
  }
  static jboolean JNICALL CallCallBooleanMethodA(JNIEnv* env, jobject obj,
                                                 jmethodID methodID,
                                                 InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallBooleanMethodA(
        obj, methodID, args);
  }
  static jboolean JNICALL CallCallBooleanMethodV(JNIEnv* env, jobject obj,
                                                 jmethodID methodID,
                                                 va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallBooleanMethodV(
        obj, methodID, args);
  }
  static jbyte JNICALL CallCallByteMethodA(JNIEnv* env, jobject obj,
                                           jmethodID methodID,
                                           InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallByteMethodA(obj, methodID,
                                                                  args);
  }
  static jbyte JNICALL CallCallByteMethodV(JNIEnv* env, jobject obj,
                                           jmethodID methodID, va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallByteMethodV(obj, methodID,
                                                                  args);
  }
  static jchar JNICALL CallCallCharMethodA(JNIEnv* env, jobject obj,
                                           jmethodID methodID,
                                           InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallCharMethodA(obj, methodID,
                                                                  args);
  }
  static jchar JNICALL CallCallCharMethodV(JNIEnv* env, jobject obj,
                                           jmethodID methodID, va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallCharMethodV(obj, methodID,
                                                                  args);
  }
  static jdouble JNICALL CallCallDoubleMethodA(JNIEnv* env, jobject obj,
                                               jmethodID methodID,
                                               InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallDoubleMethodA(
        obj, methodID, args);
  }
  static jdouble JNICALL CallCallDoubleMethodV(JNIEnv* env, jobject obj,
                                               jmethodID methodID,
                                               va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallDoubleMethodV(
        obj, methodID, args);
  }
  static jfloat JNICALL CallCallFloatMethodA(JNIEnv* env, jobject obj,
                                             jmethodID methodID,
                                             InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallFloatMethodA(
        obj, methodID, args);
  }
  static jfloat JNICALL CallCallFloatMethodV(JNIEnv* env, jobject obj,
                                             jmethodID methodID, va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallFloatMethodV(
        obj, methodID, args);
  }
  static jint JNICALL CallCallIntMethodA(JNIEnv* env, jobject obj,
                                         jmethodID methodID,
                                         InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallIntMethodA(obj, methodID,
                                                                 args);
  }
  static jint JNICALL CallCallIntMethodV(JNIEnv* env, jobject obj,
                                         jmethodID methodID, va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallIntMethodV(obj, methodID,
                                                                 args);
  }
  static jlong JNICALL CallCallLongMethodA(JNIEnv* env, jobject obj,
                                           jmethodID methodID,
                                           InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallLongMethodA(obj, methodID,
                                                                  args);
  }
  static jlong JNICALL CallCallLongMethodV(JNIEnv* env, jobject obj,
                                           jmethodID methodID, va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallLongMethodV(obj, methodID,
                                                                  args);
  }
  static jboolean JNICALL
  CallCallNonvirtualBooleanMethodA(JNIEnv* env, jobject obj, jclass clazz,
                                   jmethodID methodID, InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualBooleanMethodA(
        obj, clazz, methodID, args);
  }
  static jboolean JNICALL CallCallNonvirtualBooleanMethodV(JNIEnv* env,
                                                           jobject obj,
                                                           jclass clazz,
                                                           jmethodID methodID,
                                                           va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualBooleanMethodV(
        obj, clazz, methodID, args);
  }
  static jbyte JNICALL CallCallNonvirtualByteMethodA(JNIEnv* env, jobject obj,
                                                     jclass clazz,
                                                     jmethodID methodID,
                                                     InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualByteMethodA(
        obj, clazz, methodID, args);
  }
  static jbyte JNICALL CallCallNonvirtualByteMethodV(JNIEnv* env, jobject obj,
                                                     jclass clazz,
                                                     jmethodID methodID,
                                                     va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualByteMethodV(
        obj, clazz, methodID, args);
  }
  static jchar JNICALL CallCallNonvirtualCharMethodA(JNIEnv* env, jobject obj,
                                                     jclass clazz,
                                                     jmethodID methodID,
                                                     InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualCharMethodA(
        obj, clazz, methodID, args);
  }
  static jchar JNICALL CallCallNonvirtualCharMethodV(JNIEnv* env, jobject obj,
                                                     jclass clazz,
                                                     jmethodID methodID,
                                                     va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualCharMethodV(
        obj, clazz, methodID, args);
  }
  static jdouble JNICALL
  CallCallNonvirtualDoubleMethodA(JNIEnv* env, jobject obj, jclass clazz,
                                  jmethodID methodID, InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualDoubleMethodA(
        obj, clazz, methodID, args);
  }
  static jdouble JNICALL CallCallNonvirtualDoubleMethodV(JNIEnv* env,
                                                         jobject obj,
                                                         jclass clazz,
                                                         jmethodID methodID,
                                                         va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualDoubleMethodV(
        obj, clazz, methodID, args);
  }
  static jfloat JNICALL CallCallNonvirtualFloatMethodA(JNIEnv* env, jobject obj,
                                                       jclass clazz,
                                                       jmethodID methodID,
                                                       InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualFloatMethodA(
        obj, clazz, methodID, args);
  }
  static jfloat JNICALL CallCallNonvirtualFloatMethodV(JNIEnv* env, jobject obj,
                                                       jclass clazz,
                                                       jmethodID methodID,
                                                       va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualFloatMethodV(
        obj, clazz, methodID, args);
  }
  static jint JNICALL CallCallNonvirtualIntMethodA(JNIEnv* env, jobject obj,
                                                   jclass clazz,
                                                   jmethodID methodID,
                                                   InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualIntMethodA(
        obj, clazz, methodID, args);
  }
  static jint JNICALL CallCallNonvirtualIntMethodV(JNIEnv* env, jobject obj,
                                                   jclass clazz,
                                                   jmethodID methodID,
                                                   va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualIntMethodV(
        obj, clazz, methodID, args);
  }
  static jlong JNICALL CallCallNonvirtualLongMethodA(JNIEnv* env, jobject obj,
                                                     jclass clazz,
                                                     jmethodID methodID,
                                                     InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualLongMethodA(
        obj, clazz, methodID, args);
  }
  static jlong JNICALL CallCallNonvirtualLongMethodV(JNIEnv* env, jobject obj,
                                                     jclass clazz,
                                                     jmethodID methodID,
                                                     va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualLongMethodV(
        obj, clazz, methodID, args);
  }
  static jobject JNICALL
  CallCallNonvirtualObjectMethodA(JNIEnv* env, jobject obj, jclass clazz,
                                  jmethodID methodID, InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualObjectMethodA(
        obj, clazz, methodID, args);
  }
  static jobject JNICALL CallCallNonvirtualObjectMethodV(JNIEnv* env,
                                                         jobject obj,
                                                         jclass clazz,
                                                         jmethodID methodID,
                                                         va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualObjectMethodV(
        obj, clazz, methodID, args);
  }
  static jshort JNICALL CallCallNonvirtualShortMethodA(JNIEnv* env, jobject obj,
                                                       jclass clazz,
                                                       jmethodID methodID,
                                                       InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualShortMethodA(
        obj, clazz, methodID, args);
  }
  static jshort JNICALL CallCallNonvirtualShortMethodV(JNIEnv* env, jobject obj,
                                                       jclass clazz,
                                                       jmethodID methodID,
                                                       va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualShortMethodV(
        obj, clazz, methodID, args);
  }
  static void JNICALL CallCallNonvirtualVoidMethodA(JNIEnv* env, jobject obj,
                                                    jclass clazz,
                                                    jmethodID methodID,
                                                    InputJvalueArray args) {
    static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualVoidMethodA(
        obj, clazz, methodID, args);
  }
  static void JNICALL CallCallNonvirtualVoidMethodV(JNIEnv* env, jobject obj,
                                                    jclass clazz,
                                                    jmethodID methodID,
                                                    va_list args) {
    static_cast<MockableJNIEnvFull*>(env)->CallNonvirtualVoidMethodV(
        obj, clazz, methodID, args);
  }
  static jobject JNICALL CallCallObjectMethodA(JNIEnv* env, jobject obj,
                                               jmethodID methodID,
                                               InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallObjectMethodA(
        obj, methodID, args);
  }
  static jobject JNICALL CallCallObjectMethodV(JNIEnv* env, jobject obj,
                                               jmethodID methodID,
                                               va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallObjectMethodV(
        obj, methodID, args);
  }
  static jshort JNICALL CallCallShortMethodA(JNIEnv* env, jobject obj,
                                             jmethodID methodID,
                                             InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallShortMethodA(
        obj, methodID, args);
  }
  static jshort JNICALL CallCallShortMethodV(JNIEnv* env, jobject obj,
                                             jmethodID methodID, va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallShortMethodV(
        obj, methodID, args);
  }
  static jboolean JNICALL CallCallStaticBooleanMethodA(JNIEnv* env,
                                                       jclass clazz,
                                                       jmethodID methodID,
                                                       InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticBooleanMethodA(
        clazz, methodID, args);
  }
  static jboolean JNICALL CallCallStaticBooleanMethodV(JNIEnv* env,
                                                       jclass clazz,
                                                       jmethodID methodID,
                                                       va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticBooleanMethodV(
        clazz, methodID, args);
  }
  static jbyte JNICALL CallCallStaticByteMethodA(JNIEnv* env, jclass clazz,
                                                 jmethodID methodID,
                                                 InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticByteMethodA(
        clazz, methodID, args);
  }
  static jbyte JNICALL CallCallStaticByteMethodV(JNIEnv* env, jclass clazz,
                                                 jmethodID methodID,
                                                 va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticByteMethodV(
        clazz, methodID, args);
  }
  static jchar JNICALL CallCallStaticCharMethodA(JNIEnv* env, jclass clazz,
                                                 jmethodID methodID,
                                                 InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticCharMethodA(
        clazz, methodID, args);
  }
  static jchar JNICALL CallCallStaticCharMethodV(JNIEnv* env, jclass clazz,
                                                 jmethodID methodID,
                                                 va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticCharMethodV(
        clazz, methodID, args);
  }
  static jdouble JNICALL CallCallStaticDoubleMethodA(JNIEnv* env, jclass clazz,
                                                     jmethodID methodID,
                                                     InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticDoubleMethodA(
        clazz, methodID, args);
  }
  static jdouble JNICALL CallCallStaticDoubleMethodV(JNIEnv* env, jclass clazz,
                                                     jmethodID methodID,
                                                     va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticDoubleMethodV(
        clazz, methodID, args);
  }
  static jfloat JNICALL CallCallStaticFloatMethodA(JNIEnv* env, jclass clazz,
                                                   jmethodID methodID,
                                                   InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticFloatMethodA(
        clazz, methodID, args);
  }
  static jfloat JNICALL CallCallStaticFloatMethodV(JNIEnv* env, jclass clazz,
                                                   jmethodID methodID,
                                                   va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticFloatMethodV(
        clazz, methodID, args);
  }
  static jint JNICALL CallCallStaticIntMethodA(JNIEnv* env, jclass clazz,
                                               jmethodID methodID,
                                               InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticIntMethodA(
        clazz, methodID, args);
  }
  static jint JNICALL CallCallStaticIntMethodV(JNIEnv* env, jclass clazz,
                                               jmethodID methodID,
                                               va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticIntMethodV(
        clazz, methodID, args);
  }
  static jlong JNICALL CallCallStaticLongMethodA(JNIEnv* env, jclass clazz,
                                                 jmethodID methodID,
                                                 InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticLongMethodA(
        clazz, methodID, args);
  }
  static jlong JNICALL CallCallStaticLongMethodV(JNIEnv* env, jclass clazz,
                                                 jmethodID methodID,
                                                 va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticLongMethodV(
        clazz, methodID, args);
  }
  static jobject JNICALL CallCallStaticObjectMethodA(JNIEnv* env, jclass clazz,
                                                     jmethodID methodID,
                                                     InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticObjectMethodA(
        clazz, methodID, args);
  }
  static jobject JNICALL CallCallStaticObjectMethodV(JNIEnv* env, jclass clazz,
                                                     jmethodID methodID,
                                                     va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticObjectMethodV(
        clazz, methodID, args);
  }
  static jshort JNICALL CallCallStaticShortMethodA(JNIEnv* env, jclass clazz,
                                                   jmethodID methodID,
                                                   InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticShortMethodA(
        clazz, methodID, args);
  }
  static jshort JNICALL CallCallStaticShortMethodV(JNIEnv* env, jclass clazz,
                                                   jmethodID methodID,
                                                   va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->CallStaticShortMethodV(
        clazz, methodID, args);
  }
  static void JNICALL CallCallStaticVoidMethodA(JNIEnv* env, jclass cls,
                                                jmethodID methodID,
                                                InputJvalueArray args) {
    static_cast<MockableJNIEnvFull*>(env)->CallStaticVoidMethodA(cls, methodID,
                                                                 args);
  }
  static void JNICALL CallCallStaticVoidMethodV(JNIEnv* env, jclass cls,
                                                jmethodID methodID,
                                                va_list args) {
    static_cast<MockableJNIEnvFull*>(env)->CallStaticVoidMethodV(cls, methodID,
                                                                 args);
  }
  static void JNICALL CallCallVoidMethodA(JNIEnv* env, jobject obj,
                                          jmethodID methodID,
                                          InputJvalueArray args) {
    static_cast<MockableJNIEnvFull*>(env)->CallVoidMethodA(obj, methodID, args);
  }
  static void JNICALL CallCallVoidMethodV(JNIEnv* env, jobject obj,
                                          jmethodID methodID, va_list args) {
    static_cast<MockableJNIEnvFull*>(env)->CallVoidMethodV(obj, methodID, args);
  }
  static jclass JNICALL CallDefineClass(JNIEnv* env, const char* name,
                                        jobject loader, const jbyte* buf,
                                        jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->DefineClass(name, loader, buf,
                                                              len);
  }
  static void JNICALL CallDeleteGlobalRef(JNIEnv* env, jobject gref) {
    static_cast<MockableJNIEnvFull*>(env)->DeleteGlobalRef(gref);
  }
  static void JNICALL CallDeleteLocalRef(JNIEnv* env, jobject obj) {
    static_cast<MockableJNIEnvFull*>(env)->DeleteLocalRef(obj);
  }
  static void JNICALL CallDeleteWeakGlobalRef(JNIEnv* env, jweak ref) {
    static_cast<MockableJNIEnvFull*>(env)->DeleteWeakGlobalRef(ref);
  }
  static jint JNICALL CallEnsureLocalCapacity(JNIEnv* env, jint capacity) {
    return static_cast<MockableJNIEnvFull*>(env)->EnsureLocalCapacity(capacity);
  }
  static jboolean JNICALL CallExceptionCheck(JNIEnv* env) {
    return static_cast<MockableJNIEnvFull*>(env)->ExceptionCheck();
  }
  static void JNICALL CallExceptionClear(JNIEnv* env) {
    static_cast<MockableJNIEnvFull*>(env)->ExceptionClear();
  }
  static void JNICALL CallExceptionDescribe(JNIEnv* env) {
    static_cast<MockableJNIEnvFull*>(env)->ExceptionDescribe();
  }
  static jthrowable JNICALL CallExceptionOccurred(JNIEnv* env) {
    return static_cast<MockableJNIEnvFull*>(env)->ExceptionOccurred();
  }
  static void JNICALL CallFatalError(JNIEnv* env, const char* msg) {
    static_cast<MockableJNIEnvFull*>(env)->FatalError(msg);
  }
  static jclass JNICALL CallFindClass(JNIEnv* env, const char* name) {
    return static_cast<MockableJNIEnvFull*>(env)->FindClass(name);
  }
  static jfieldID JNICALL CallFromReflectedField(JNIEnv* env, jobject field) {
    return static_cast<MockableJNIEnvFull*>(env)->FromReflectedField(field);
  }
  static jmethodID JNICALL CallFromReflectedMethod(JNIEnv* env,
                                                   jobject method) {
    return static_cast<MockableJNIEnvFull*>(env)->FromReflectedMethod(method);
  }
  static jsize JNICALL CallGetArrayLength(JNIEnv* env, jarray array) {
    return static_cast<MockableJNIEnvFull*>(env)->GetArrayLength(array);
  }
  static jboolean* JNICALL CallGetBooleanArrayElements(JNIEnv* env,
                                                       jbooleanArray array,
                                                       jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetBooleanArrayElements(
        array, isCopy);
  }
  static void JNICALL CallGetBooleanArrayRegion(JNIEnv* env,
                                                jbooleanArray array,
                                                jsize start, jsize len,
                                                jboolean* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetBooleanArrayRegion(array, start,
                                                                 len, buf);
  }
  static jboolean JNICALL CallGetBooleanField(JNIEnv* env, jobject obj,
                                              jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetBooleanField(obj, fieldID);
  }
  static jbyte* JNICALL CallGetByteArrayElements(JNIEnv* env, jbyteArray array,
                                                 jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetByteArrayElements(array,
                                                                       isCopy);
  }
  static void JNICALL CallGetByteArrayRegion(JNIEnv* env, jbyteArray array,
                                             jsize start, jsize len,
                                             jbyte* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetByteArrayRegion(array, start, len,
                                                              buf);
  }
  static jbyte JNICALL CallGetByteField(JNIEnv* env, jobject obj,
                                        jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetByteField(obj, fieldID);
  }
  static jchar* JNICALL CallGetCharArrayElements(JNIEnv* env, jcharArray array,
                                                 jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetCharArrayElements(array,
                                                                       isCopy);
  }
  static void JNICALL CallGetCharArrayRegion(JNIEnv* env, jcharArray array,
                                             jsize start, jsize len,
                                             jchar* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetCharArrayRegion(array, start, len,
                                                              buf);
  }
  static jchar JNICALL CallGetCharField(JNIEnv* env, jobject obj,
                                        jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetCharField(obj, fieldID);
  }
  static void* JNICALL CallGetDirectBufferAddress(JNIEnv* env, jobject buf) {
    return static_cast<MockableJNIEnvFull*>(env)->GetDirectBufferAddress(buf);
  }
  static jlong JNICALL CallGetDirectBufferCapacity(JNIEnv* env, jobject buf) {
    return static_cast<MockableJNIEnvFull*>(env)->GetDirectBufferCapacity(buf);
  }
  static jdouble* JNICALL CallGetDoubleArrayElements(JNIEnv* env,
                                                     jdoubleArray array,
                                                     jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetDoubleArrayElements(
        array, isCopy);
  }
  static void JNICALL CallGetDoubleArrayRegion(JNIEnv* env, jdoubleArray array,
                                               jsize start, jsize len,
                                               jdouble* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetDoubleArrayRegion(array, start,
                                                                len, buf);
  }
  static jdouble JNICALL CallGetDoubleField(JNIEnv* env, jobject obj,
                                            jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetDoubleField(obj, fieldID);
  }
  static jfieldID JNICALL CallGetFieldID(JNIEnv* env, jclass clazz,
                                         const char* name, const char* sig) {
    return static_cast<MockableJNIEnvFull*>(env)->GetFieldID(clazz, name, sig);
  }
  static jfloat* JNICALL CallGetFloatArrayElements(JNIEnv* env,
                                                   jfloatArray array,
                                                   jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetFloatArrayElements(array,
                                                                        isCopy);
  }
  static void JNICALL CallGetFloatArrayRegion(JNIEnv* env, jfloatArray array,
                                              jsize start, jsize len,
                                              jfloat* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetFloatArrayRegion(array, start,
                                                               len, buf);
  }
  static jfloat JNICALL CallGetFloatField(JNIEnv* env, jobject obj,
                                          jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetFloatField(obj, fieldID);
  }
  static jint* JNICALL CallGetIntArrayElements(JNIEnv* env, jintArray array,
                                               jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetIntArrayElements(array,
                                                                      isCopy);
  }
  static void JNICALL CallGetIntArrayRegion(JNIEnv* env, jintArray array,
                                            jsize start, jsize len, jint* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetIntArrayRegion(array, start, len,
                                                             buf);
  }
  static jint JNICALL CallGetIntField(JNIEnv* env, jobject obj,
                                      jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetIntField(obj, fieldID);
  }
  static jint JNICALL CallGetJavaVM(JNIEnv* env, JavaVM** vm) {
    return static_cast<MockableJNIEnvFull*>(env)->GetJavaVM(vm);
  }
  static jlong* JNICALL CallGetLongArrayElements(JNIEnv* env, jlongArray array,
                                                 jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetLongArrayElements(array,
                                                                       isCopy);
  }
  static void JNICALL CallGetLongArrayRegion(JNIEnv* env, jlongArray array,
                                             jsize start, jsize len,
                                             jlong* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetLongArrayRegion(array, start, len,
                                                              buf);
  }
  static jlong JNICALL CallGetLongField(JNIEnv* env, jobject obj,
                                        jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetLongField(obj, fieldID);
  }
  static jmethodID JNICALL CallGetMethodID(JNIEnv* env, jclass clazz,
                                           const char* name, const char* sig) {
    return static_cast<MockableJNIEnvFull*>(env)->GetMethodID(clazz, name, sig);
  }
  static jobject JNICALL CallGetObjectArrayElement(JNIEnv* env,
                                                   jobjectArray array,
                                                   jsize index) {
    return static_cast<MockableJNIEnvFull*>(env)->GetObjectArrayElement(array,
                                                                        index);
  }
  static jclass JNICALL CallGetObjectClass(JNIEnv* env, jobject obj) {
    return static_cast<MockableJNIEnvFull*>(env)->GetObjectClass(obj);
  }
  static jobject JNICALL CallGetObjectField(JNIEnv* env, jobject obj,
                                            jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetObjectField(obj, fieldID);
  }
  static jobjectRefType JNICALL CallGetObjectRefType(JNIEnv* env, jobject obj) {
    return static_cast<MockableJNIEnvFull*>(env)->GetObjectRefType(obj);
  }
  static void* JNICALL CallGetPrimitiveArrayCritical(JNIEnv* env, jarray array,
                                                     jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetPrimitiveArrayCritical(
        array, isCopy);
  }
  static jshort* JNICALL CallGetShortArrayElements(JNIEnv* env,
                                                   jshortArray array,
                                                   jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetShortArrayElements(array,
                                                                        isCopy);
  }
  static void JNICALL CallGetShortArrayRegion(JNIEnv* env, jshortArray array,
                                              jsize start, jsize len,
                                              jshort* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetShortArrayRegion(array, start,
                                                               len, buf);
  }
  static jshort JNICALL CallGetShortField(JNIEnv* env, jobject obj,
                                          jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetShortField(obj, fieldID);
  }
  static jboolean JNICALL CallGetStaticBooleanField(JNIEnv* env, jclass clazz,
                                                    jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticBooleanField(
        clazz, fieldID);
  }
  static jbyte JNICALL CallGetStaticByteField(JNIEnv* env, jclass clazz,
                                              jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticByteField(clazz,
                                                                     fieldID);
  }
  static jchar JNICALL CallGetStaticCharField(JNIEnv* env, jclass clazz,
                                              jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticCharField(clazz,
                                                                     fieldID);
  }
  static jdouble JNICALL CallGetStaticDoubleField(JNIEnv* env, jclass clazz,
                                                  jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticDoubleField(clazz,
                                                                       fieldID);
  }
  static jfieldID JNICALL CallGetStaticFieldID(JNIEnv* env, jclass clazz,
                                               const char* name,
                                               const char* sig) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticFieldID(clazz, name,
                                                                   sig);
  }
  static jfloat JNICALL CallGetStaticFloatField(JNIEnv* env, jclass clazz,
                                                jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticFloatField(clazz,
                                                                      fieldID);
  }
  static jint JNICALL CallGetStaticIntField(JNIEnv* env, jclass clazz,
                                            jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticIntField(clazz,
                                                                    fieldID);
  }
  static jlong JNICALL CallGetStaticLongField(JNIEnv* env, jclass clazz,
                                              jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticLongField(clazz,
                                                                     fieldID);
  }
  static jmethodID JNICALL CallGetStaticMethodID(JNIEnv* env, jclass clazz,
                                                 const char* name,
                                                 const char* sig) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticMethodID(clazz, name,
                                                                    sig);
  }
  static jobject JNICALL CallGetStaticObjectField(JNIEnv* env, jclass clazz,
                                                  jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticObjectField(clazz,
                                                                       fieldID);
  }
  static jshort JNICALL CallGetStaticShortField(JNIEnv* env, jclass clazz,
                                                jfieldID fieldID) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStaticShortField(clazz,
                                                                      fieldID);
  }
  static jsize JNICALL CallGetStringLength(JNIEnv* env, jstring str) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStringLength(str);
  }
  static void JNICALL CallGetStringRegion(JNIEnv* env, jstring str, jsize start,
                                          jsize len, jchar* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetStringRegion(str, start, len,
                                                           buf);
  }
  static const char* JNICALL CallGetStringUTFChars(JNIEnv* env, jstring str,
                                                   jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStringUTFChars(str,
                                                                    isCopy);
  }
  static jsize JNICALL CallGetStringUTFLength(JNIEnv* env, jstring str) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStringUTFLength(str);
  }
  static void JNICALL CallGetStringUTFRegion(JNIEnv* env, jstring str,
                                             jsize start, jsize len,
                                             char* buf) {
    static_cast<MockableJNIEnvFull*>(env)->GetStringUTFRegion(str, start, len,
                                                              buf);
  }
  static jclass JNICALL CallGetSuperclass(JNIEnv* env, jclass sub) {
    return static_cast<MockableJNIEnvFull*>(env)->GetSuperclass(sub);
  }
  static jint JNICALL CallGetVersion(JNIEnv* env) {
    return static_cast<MockableJNIEnvFull*>(env)->GetVersion();
  }
  static jboolean JNICALL CallIsAssignableFrom(JNIEnv* env, jclass sub,
                                               jclass sup) {
    return static_cast<MockableJNIEnvFull*>(env)->IsAssignableFrom(sub, sup);
  }
  static jboolean JNICALL CallIsInstanceOf(JNIEnv* env, jobject obj,
                                           jclass clazz) {
    return static_cast<MockableJNIEnvFull*>(env)->IsInstanceOf(obj, clazz);
  }
  static jboolean JNICALL CallIsSameObject(JNIEnv* env, jobject obj1,
                                           jobject obj2) {
    return static_cast<MockableJNIEnvFull*>(env)->IsSameObject(obj1, obj2);
  }
  static const jchar* JNICALL CallGetStringChars(JNIEnv* env, jstring str,
                                                 jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStringChars(str, isCopy);
  }
  static const jchar* JNICALL CallGetStringCritical(JNIEnv* env, jstring str,
                                                    jboolean* isCopy) {
    return static_cast<MockableJNIEnvFull*>(env)->GetStringCritical(str,
                                                                    isCopy);
  }
  static jint JNICALL CallMonitorEnter(JNIEnv* env, jobject obj) {
    return static_cast<MockableJNIEnvFull*>(env)->MonitorEnter(obj);
  }
  static jint JNICALL CallMonitorExit(JNIEnv* env, jobject obj) {
    return static_cast<MockableJNIEnvFull*>(env)->MonitorExit(obj);
  }
  static jbooleanArray JNICALL CallNewBooleanArray(JNIEnv* env, jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewBooleanArray(len);
  }
  static jbyteArray JNICALL CallNewByteArray(JNIEnv* env, jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewByteArray(len);
  }
  static jcharArray JNICALL CallNewCharArray(JNIEnv* env, jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewCharArray(len);
  }
  static jobject JNICALL CallNewDirectByteBuffer(JNIEnv* env, void* address,
                                                 jlong capacity) {
    return static_cast<MockableJNIEnvFull*>(env)->NewDirectByteBuffer(address,
                                                                      capacity);
  }
  static jdoubleArray JNICALL CallNewDoubleArray(JNIEnv* env, jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewDoubleArray(len);
  }
  static jfloatArray JNICALL CallNewFloatArray(JNIEnv* env, jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewFloatArray(len);
  }
  static jobject JNICALL CallNewGlobalRef(JNIEnv* env, jobject lobj) {
    return static_cast<MockableJNIEnvFull*>(env)->NewGlobalRef(lobj);
  }
  static jintArray JNICALL CallNewIntArray(JNIEnv* env, jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewIntArray(len);
  }
  static jobject JNICALL CallNewLocalRef(JNIEnv* env, jobject ref) {
    return static_cast<MockableJNIEnvFull*>(env)->NewLocalRef(ref);
  }
  static jlongArray JNICALL CallNewLongArray(JNIEnv* env, jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewLongArray(len);
  }
  static jobject JNICALL CallNewObjectA(JNIEnv* env, jclass clazz,
                                        jmethodID methodID,
                                        InputJvalueArray args) {
    return static_cast<MockableJNIEnvFull*>(env)->NewObjectA(clazz, methodID,
                                                             args);
  }
  static jobjectArray JNICALL CallNewObjectArray(JNIEnv* env, jsize len,
                                                 jclass clazz, jobject init) {
    return static_cast<MockableJNIEnvFull*>(env)->NewObjectArray(len, clazz,
                                                                 init);
  }
  static jobject JNICALL CallNewObjectV(JNIEnv* env, jclass clazz,
                                        jmethodID methodID, va_list args) {
    return static_cast<MockableJNIEnvFull*>(env)->NewObjectV(clazz, methodID,
                                                             args);
  }
  static jshortArray JNICALL CallNewShortArray(JNIEnv* env, jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewShortArray(len);
  }
  static jstring JNICALL CallNewString(JNIEnv* env, const jchar* unicode,
                                       jsize len) {
    return static_cast<MockableJNIEnvFull*>(env)->NewString(unicode, len);
  }
  static jstring JNICALL CallNewStringUTF(JNIEnv* env, const char* utf) {
    return static_cast<MockableJNIEnvFull*>(env)->NewStringUTF(utf);
  }
  static jweak JNICALL CallNewWeakGlobalRef(JNIEnv* env, jobject obj) {
    return static_cast<MockableJNIEnvFull*>(env)->NewWeakGlobalRef(obj);
  }
  static jobject JNICALL CallPopLocalFrame(JNIEnv* env, jobject result) {
    return static_cast<MockableJNIEnvFull*>(env)->PopLocalFrame(result);
  }
  static jint JNICALL CallPushLocalFrame(JNIEnv* env, jint capacity) {
    return static_cast<MockableJNIEnvFull*>(env)->PushLocalFrame(capacity);
  }
  static jint JNICALL CallRegisterNatives(JNIEnv* env, jclass clazz,
                                          const JNINativeMethod* methods,
                                          jint nMethods) {
    return static_cast<MockableJNIEnvFull*>(env)->RegisterNatives(
        clazz, methods, nMethods);
  }
  static void JNICALL CallReleaseBooleanArrayElements(JNIEnv* env,
                                                      jbooleanArray array,
                                                      jboolean* elems,
                                                      jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseBooleanArrayElements(
        array, elems, mode);
  }
  static void JNICALL CallReleaseByteArrayElements(JNIEnv* env,
                                                   jbyteArray array,
                                                   jbyte* elems, jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseByteArrayElements(
        array, elems, mode);
  }
  static void JNICALL CallReleaseCharArrayElements(JNIEnv* env,
                                                   jcharArray array,
                                                   jchar* elems, jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseCharArrayElements(
        array, elems, mode);
  }
  static void JNICALL CallReleaseDoubleArrayElements(JNIEnv* env,
                                                     jdoubleArray array,
                                                     jdouble* elems,
                                                     jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseDoubleArrayElements(
        array, elems, mode);
  }
  static void JNICALL CallReleaseFloatArrayElements(JNIEnv* env,
                                                    jfloatArray array,
                                                    jfloat* elems, jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseFloatArrayElements(
        array, elems, mode);
  }
  static void JNICALL CallReleaseIntArrayElements(JNIEnv* env, jintArray array,
                                                  jint* elems, jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseIntArrayElements(array, elems,
                                                                   mode);
  }
  static void JNICALL CallReleaseLongArrayElements(JNIEnv* env,
                                                   jlongArray array,
                                                   jlong* elems, jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseLongArrayElements(
        array, elems, mode);
  }
  static void JNICALL CallReleasePrimitiveArrayCritical(JNIEnv* env,
                                                        jarray array,
                                                        void* carray,
                                                        jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleasePrimitiveArrayCritical(
        array, carray, mode);
  }
  static void JNICALL CallReleaseShortArrayElements(JNIEnv* env,
                                                    jshortArray array,
                                                    jshort* elems, jint mode) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseShortArrayElements(
        array, elems, mode);
  }
  static void JNICALL CallReleaseStringChars(JNIEnv* env, jstring str,
                                             const jchar* chars) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseStringChars(str, chars);
  }
  static void JNICALL CallReleaseStringCritical(JNIEnv* env, jstring str,
                                                const jchar* cstring) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseStringCritical(str, cstring);
  }
  static void JNICALL CallReleaseStringUTFChars(JNIEnv* env, jstring str,
                                                const char* chars) {
    static_cast<MockableJNIEnvFull*>(env)->ReleaseStringUTFChars(str, chars);
  }
  static void JNICALL CallSetBooleanArrayRegion(JNIEnv* env,
                                                jbooleanArray array,
                                                jsize start, jsize len,
                                                const jboolean* buf) {
    static_cast<MockableJNIEnvFull*>(env)->SetBooleanArrayRegion(array, start,
                                                                 len, buf);
  }
  static void JNICALL CallSetBooleanField(JNIEnv* env, jobject obj,
                                          jfieldID fieldID, jboolean val) {
    static_cast<MockableJNIEnvFull*>(env)->SetBooleanField(obj, fieldID, val);
  }
  static void JNICALL CallSetByteArrayRegion(JNIEnv* env, jbyteArray array,
                                             jsize start, jsize len,
                                             const jbyte* buf) {
    static_cast<MockableJNIEnvFull*>(env)->SetByteArrayRegion(array, start, len,
                                                              buf);
  }
  static void JNICALL CallSetByteField(JNIEnv* env, jobject obj,
                                       jfieldID fieldID, jbyte val) {
    static_cast<MockableJNIEnvFull*>(env)->SetByteField(obj, fieldID, val);
  }
  static void JNICALL CallSetCharArrayRegion(JNIEnv* env, jcharArray array,
                                             jsize start, jsize len,
                                             const jchar* buf) {
    static_cast<MockableJNIEnvFull*>(env)->SetCharArrayRegion(array, start, len,
                                                              buf);
  }
  static void JNICALL CallSetCharField(JNIEnv* env, jobject obj,
                                       jfieldID fieldID, jchar val) {
    static_cast<MockableJNIEnvFull*>(env)->SetCharField(obj, fieldID, val);
  }
  static void JNICALL CallSetDoubleArrayRegion(JNIEnv* env, jdoubleArray array,
                                               jsize start, jsize len,
                                               const jdouble* buf) {
    static_cast<MockableJNIEnvFull*>(env)->SetDoubleArrayRegion(array, start,
                                                                len, buf);
  }
  static void JNICALL CallSetDoubleField(JNIEnv* env, jobject obj,
                                         jfieldID fieldID, jdouble val) {
    static_cast<MockableJNIEnvFull*>(env)->SetDoubleField(obj, fieldID, val);
  }
  static void JNICALL CallSetFloatArrayRegion(JNIEnv* env, jfloatArray array,
                                              jsize start, jsize len,
                                              const jfloat* buf) {
    static_cast<MockableJNIEnvFull*>(env)->SetFloatArrayRegion(array, start,
                                                               len, buf);
  }
  static void JNICALL CallSetFloatField(JNIEnv* env, jobject obj,
                                        jfieldID fieldID, jfloat val) {
    static_cast<MockableJNIEnvFull*>(env)->SetFloatField(obj, fieldID, val);
  }
  static void JNICALL CallSetIntArrayRegion(JNIEnv* env, jintArray array,
                                            jsize start, jsize len,
                                            const jint* buf) {
    static_cast<MockableJNIEnvFull*>(env)->SetIntArrayRegion(array, start, len,
                                                             buf);
  }
  static void JNICALL CallSetIntField(JNIEnv* env, jobject obj,
                                      jfieldID fieldID, jint val) {
    static_cast<MockableJNIEnvFull*>(env)->SetIntField(obj, fieldID, val);
  }
  static void JNICALL CallSetLongArrayRegion(JNIEnv* env, jlongArray array,
                                             jsize start, jsize len,
                                             const jlong* buf) {
    static_cast<MockableJNIEnvFull*>(env)->SetLongArrayRegion(array, start, len,
                                                              buf);
  }
  static void JNICALL CallSetLongField(JNIEnv* env, jobject obj,
                                       jfieldID fieldID, jlong val) {
    static_cast<MockableJNIEnvFull*>(env)->SetLongField(obj, fieldID, val);
  }
  static void JNICALL CallSetObjectArrayElement(JNIEnv* env, jobjectArray array,
                                                jsize index, jobject val) {
    static_cast<MockableJNIEnvFull*>(env)->SetObjectArrayElement(array, index,
                                                                 val);
  }
  static void JNICALL CallSetObjectField(JNIEnv* env, jobject obj,
                                         jfieldID fieldID, jobject val) {
    static_cast<MockableJNIEnvFull*>(env)->SetObjectField(obj, fieldID, val);
  }
  static void JNICALL CallSetShortArrayRegion(JNIEnv* env, jshortArray array,
                                              jsize start, jsize len,
                                              const jshort* buf) {
    static_cast<MockableJNIEnvFull*>(env)->SetShortArrayRegion(array, start,
                                                               len, buf);
  }
  static void JNICALL CallSetShortField(JNIEnv* env, jobject obj,
                                        jfieldID fieldID, jshort val) {
    static_cast<MockableJNIEnvFull*>(env)->SetShortField(obj, fieldID, val);
  }
  static void JNICALL CallSetStaticBooleanField(JNIEnv* env, jclass clazz,
                                                jfieldID fieldID,
                                                jboolean value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticBooleanField(clazz, fieldID,
                                                                 value);
  }
  static void JNICALL CallSetStaticByteField(JNIEnv* env, jclass clazz,
                                             jfieldID fieldID, jbyte value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticByteField(clazz, fieldID,
                                                              value);
  }
  static void JNICALL CallSetStaticCharField(JNIEnv* env, jclass clazz,
                                             jfieldID fieldID, jchar value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticCharField(clazz, fieldID,
                                                              value);
  }
  static void JNICALL CallSetStaticDoubleField(JNIEnv* env, jclass clazz,
                                               jfieldID fieldID,
                                               jdouble value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticDoubleField(clazz, fieldID,
                                                                value);
  }
  static void JNICALL CallSetStaticFloatField(JNIEnv* env, jclass clazz,
                                              jfieldID fieldID, jfloat value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticFloatField(clazz, fieldID,
                                                               value);
  }
  static void JNICALL CallSetStaticIntField(JNIEnv* env, jclass clazz,
                                            jfieldID fieldID, jint value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticIntField(clazz, fieldID,
                                                             value);
  }
  static void JNICALL CallSetStaticLongField(JNIEnv* env, jclass clazz,
                                             jfieldID fieldID, jlong value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticLongField(clazz, fieldID,
                                                              value);
  }
  static void JNICALL CallSetStaticObjectField(JNIEnv* env, jclass clazz,
                                               jfieldID fieldID,
                                               jobject value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticObjectField(clazz, fieldID,
                                                                value);
  }
  static void JNICALL CallSetStaticShortField(JNIEnv* env, jclass clazz,
                                              jfieldID fieldID, jshort value) {
    static_cast<MockableJNIEnvFull*>(env)->SetStaticShortField(clazz, fieldID,
                                                               value);
  }
  static jint JNICALL CallThrow(JNIEnv* env, jthrowable obj) {
    return static_cast<MockableJNIEnvFull*>(env)->Throw(obj);
  }
  static jint JNICALL CallThrowNew(JNIEnv* env, jclass clazz, const char* msg) {
    return static_cast<MockableJNIEnvFull*>(env)->ThrowNew(clazz, msg);
  }
  static jobject JNICALL CallToReflectedField(JNIEnv* env, jclass cls,
                                              jfieldID fieldID,
                                              jboolean isStatic) {
    return static_cast<MockableJNIEnvFull*>(env)->ToReflectedField(cls, fieldID,
                                                                   isStatic);
  }
  static jobject JNICALL CallToReflectedMethod(JNIEnv* env, jclass cls,
                                               jmethodID methodID,
                                               jboolean isStatic) {
    return static_cast<MockableJNIEnvFull*>(env)->ToReflectedMethod(
        cls, methodID, isStatic);
  }
  static jint JNICALL CallUnregisterNatives(JNIEnv* env, jclass clazz) {
    return static_cast<MockableJNIEnvFull*>(env)->UnregisterNatives(clazz);
  }

#ifndef __ANDROID__
  JNINativeInterface_ functions_;
#else
  JNINativeInterface functions_;
#endif
};

class MockJniEnv : public MockableJNIEnvFull {
 public:
  MOCK_METHOD(jobject, AllocObject, (jclass clazz), (override));
  MOCK_METHOD(jboolean, CallBooleanMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jboolean, CallBooleanMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jbyte, CallByteMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jbyte, CallByteMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jchar, CallCharMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jchar, CallCharMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jdouble, CallDoubleMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jdouble, CallDoubleMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jfloat, CallFloatMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jfloat, CallFloatMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jint, CallIntMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jint, CallIntMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jlong, CallLongMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jlong, CallLongMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jboolean, CallNonvirtualBooleanMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jboolean, CallNonvirtualBooleanMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jbyte, CallNonvirtualByteMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jbyte, CallNonvirtualByteMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jchar, CallNonvirtualCharMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jchar, CallNonvirtualCharMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jdouble, CallNonvirtualDoubleMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jdouble, CallNonvirtualDoubleMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jfloat, CallNonvirtualFloatMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jfloat, CallNonvirtualFloatMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jint, CallNonvirtualIntMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jint, CallNonvirtualIntMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jlong, CallNonvirtualLongMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jlong, CallNonvirtualLongMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jobject, CallNonvirtualObjectMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jobject, CallNonvirtualObjectMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jshort, CallNonvirtualShortMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(jshort, CallNonvirtualShortMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(void, CallNonvirtualVoidMethodA,
              (jobject obj, jclass clazz, jmethodID methodID,
               InputJvalueArray args),
              (override));
  MOCK_METHOD(void, CallNonvirtualVoidMethodV,
              (jobject obj, jclass clazz, jmethodID methodID, va_list args),
              (override));
  MOCK_METHOD(jobject, CallObjectMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jobject, CallObjectMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jshort, CallShortMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jshort, CallShortMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jboolean, CallStaticBooleanMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jboolean, CallStaticBooleanMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jbyte, CallStaticByteMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jbyte, CallStaticByteMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jchar, CallStaticCharMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jchar, CallStaticCharMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jdouble, CallStaticDoubleMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jdouble, CallStaticDoubleMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jfloat, CallStaticFloatMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jfloat, CallStaticFloatMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jint, CallStaticIntMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jint, CallStaticIntMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jlong, CallStaticLongMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jlong, CallStaticLongMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jobject, CallStaticObjectMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jobject, CallStaticObjectMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jshort, CallStaticShortMethodA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jshort, CallStaticShortMethodV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(void, CallStaticVoidMethodA,
              (jclass cls, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(void, CallStaticVoidMethodV,
              (jclass cls, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(void, CallVoidMethodA,
              (jobject obj, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(void, CallVoidMethodV,
              (jobject obj, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jclass, DefineClass,
              (const char* name, jobject loader, const jbyte* buf, jsize len),
              (override));
  MOCK_METHOD(void, DeleteGlobalRef, (jobject gref), (override));
  MOCK_METHOD(void, DeleteLocalRef, (jobject obj), (override));
  MOCK_METHOD(void, DeleteWeakGlobalRef, (jweak ref), (override));
  MOCK_METHOD(jint, EnsureLocalCapacity, (jint capacity), (override));
  MOCK_METHOD(jboolean, ExceptionCheck, (), (override));
  MOCK_METHOD(void, ExceptionClear, (), (override));
  MOCK_METHOD(void, ExceptionDescribe, (), (override));
  MOCK_METHOD(jthrowable, ExceptionOccurred, (), (override));
  MOCK_METHOD(void, FatalError, (const char* msg), (override));
  MOCK_METHOD(jclass, FindClass, (const char* name), (override));
  MOCK_METHOD(jfieldID, FromReflectedField, (jobject field), (override));
  MOCK_METHOD(jmethodID, FromReflectedMethod, (jobject method), (override));
  MOCK_METHOD(jsize, GetArrayLength, (jarray array), (override));
  MOCK_METHOD(jboolean*, GetBooleanArrayElements,
              (jbooleanArray array, jboolean* isCopy), (override));
  MOCK_METHOD(void, GetBooleanArrayRegion,
              (jbooleanArray array, jsize start, jsize len, jboolean* buf),
              (override));
  MOCK_METHOD(jboolean, GetBooleanField, (jobject obj, jfieldID fieldID),
              (override));
  MOCK_METHOD(jbyte*, GetByteArrayElements,
              (jbyteArray array, jboolean* isCopy), (override));
  MOCK_METHOD(void, GetByteArrayRegion,
              (jbyteArray array, jsize start, jsize len, jbyte* buf),
              (override));
  MOCK_METHOD(jbyte, GetByteField, (jobject obj, jfieldID fieldID), (override));
  MOCK_METHOD(jchar*, GetCharArrayElements,
              (jcharArray array, jboolean* isCopy), (override));
  MOCK_METHOD(void, GetCharArrayRegion,
              (jcharArray array, jsize start, jsize len, jchar* buf),
              (override));
  MOCK_METHOD(jchar, GetCharField, (jobject obj, jfieldID fieldID), (override));
  MOCK_METHOD(void*, GetDirectBufferAddress, (jobject buf), (override));
  MOCK_METHOD(jlong, GetDirectBufferCapacity, (jobject buf), (override));
  MOCK_METHOD(jdouble*, GetDoubleArrayElements,
              (jdoubleArray array, jboolean* isCopy), (override));
  MOCK_METHOD(void, GetDoubleArrayRegion,
              (jdoubleArray array, jsize start, jsize len, jdouble* buf),
              (override));
  MOCK_METHOD(jdouble, GetDoubleField, (jobject obj, jfieldID fieldID),
              (override));
  MOCK_METHOD(jfieldID, GetFieldID,
              (jclass clazz, const char* name, const char* sig), (override));
  MOCK_METHOD(jfloat*, GetFloatArrayElements,
              (jfloatArray array, jboolean* isCopy), (override));
  MOCK_METHOD(void, GetFloatArrayRegion,
              (jfloatArray array, jsize start, jsize len, jfloat* buf),
              (override));
  MOCK_METHOD(jfloat, GetFloatField, (jobject obj, jfieldID fieldID),
              (override));
  MOCK_METHOD(jint*, GetIntArrayElements, (jintArray array, jboolean* isCopy),
              (override));
  MOCK_METHOD(void, GetIntArrayRegion,
              (jintArray array, jsize start, jsize len, jint* buf), (override));
  MOCK_METHOD(jint, GetIntField, (jobject obj, jfieldID fieldID), (override));
  MOCK_METHOD(jint, GetJavaVM, (JavaVM * *vm), (override));
  MOCK_METHOD(jlong*, GetLongArrayElements,
              (jlongArray array, jboolean* isCopy), (override));
  MOCK_METHOD(void, GetLongArrayRegion,
              (jlongArray array, jsize start, jsize len, jlong* buf),
              (override));
  MOCK_METHOD(jlong, GetLongField, (jobject obj, jfieldID fieldID), (override));
  MOCK_METHOD(jmethodID, GetMethodID,
              (jclass clazz, const char* name, const char* sig), (override));
  MOCK_METHOD(jobject, GetObjectArrayElement, (jobjectArray array, jsize index),
              (override));
  MOCK_METHOD(jclass, GetObjectClass, (jobject obj), (override));
  MOCK_METHOD(jobject, GetObjectField, (jobject obj, jfieldID fieldID),
              (override));
  MOCK_METHOD(jobjectRefType, GetObjectRefType, (jobject obj), (override));
  MOCK_METHOD(void*, GetPrimitiveArrayCritical,
              (jarray array, jboolean* isCopy), (override));
  MOCK_METHOD(jshort*, GetShortArrayElements,
              (jshortArray array, jboolean* isCopy), (override));
  MOCK_METHOD(void, GetShortArrayRegion,
              (jshortArray array, jsize start, jsize len, jshort* buf),
              (override));
  MOCK_METHOD(jshort, GetShortField, (jobject obj, jfieldID fieldID),
              (override));
  MOCK_METHOD(jboolean, GetStaticBooleanField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jbyte, GetStaticByteField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jchar, GetStaticCharField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jdouble, GetStaticDoubleField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jfieldID, GetStaticFieldID,
              (jclass clazz, const char* name, const char* sig), (override));
  MOCK_METHOD(jfloat, GetStaticFloatField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jint, GetStaticIntField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jlong, GetStaticLongField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jmethodID, GetStaticMethodID,
              (jclass clazz, const char* name, const char* sig), (override));
  MOCK_METHOD(jobject, GetStaticObjectField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jshort, GetStaticShortField, (jclass clazz, jfieldID fieldID),
              (override));
  MOCK_METHOD(jsize, GetStringLength, (jstring str), (override));
  MOCK_METHOD(void, GetStringRegion,
              (jstring str, jsize start, jsize len, jchar* buf), (override));
  MOCK_METHOD(const char*, GetStringUTFChars, (jstring str, jboolean* isCopy),
              (override));
  MOCK_METHOD(jsize, GetStringUTFLength, (jstring str), (override));
  MOCK_METHOD(void, GetStringUTFRegion,
              (jstring str, jsize start, jsize len, char* buf), (override));
  MOCK_METHOD(jclass, GetSuperclass, (jclass sub), (override));
  MOCK_METHOD(jint, GetVersion, (), (override));
  MOCK_METHOD(jboolean, IsAssignableFrom, (jclass sub, jclass sup), (override));
  MOCK_METHOD(jboolean, IsInstanceOf, (jobject obj, jclass clazz), (override));
  MOCK_METHOD(jboolean, IsSameObject, (jobject obj1, jobject obj2), (override));
  MOCK_METHOD(const jchar*, GetStringChars, (jstring str, jboolean* isCopy),
              (override));
  MOCK_METHOD(const jchar*, GetStringCritical, (jstring str, jboolean* isCopy),
              (override));
  MOCK_METHOD(jint, MonitorEnter, (jobject obj), (override));
  MOCK_METHOD(jint, MonitorExit, (jobject obj), (override));
  MOCK_METHOD(jbooleanArray, NewBooleanArray, (jsize len), (override));
  MOCK_METHOD(jbyteArray, NewByteArray, (jsize len), (override));
  MOCK_METHOD(jcharArray, NewCharArray, (jsize len), (override));
  MOCK_METHOD(jobject, NewDirectByteBuffer, (void* address, jlong capacity),
              (override));
  MOCK_METHOD(jdoubleArray, NewDoubleArray, (jsize len), (override));
  MOCK_METHOD(jfloatArray, NewFloatArray, (jsize len), (override));
  MOCK_METHOD(jobject, NewGlobalRef, (jobject lobj), (override));
  MOCK_METHOD(jintArray, NewIntArray, (jsize len), (override));
  MOCK_METHOD(jobject, NewLocalRef, (jobject ref), (override));
  MOCK_METHOD(jlongArray, NewLongArray, (jsize len), (override));
  MOCK_METHOD(jobject, NewObjectA,
              (jclass clazz, jmethodID methodID, InputJvalueArray args),
              (override));
  MOCK_METHOD(jobjectArray, NewObjectArray,
              (jsize len, jclass clazz, jobject init), (override));
  MOCK_METHOD(jobject, NewObjectV,
              (jclass clazz, jmethodID methodID, va_list args), (override));
  MOCK_METHOD(jshortArray, NewShortArray, (jsize len), (override));
  MOCK_METHOD(jstring, NewString, (const jchar* unicode, jsize len),
              (override));
  MOCK_METHOD(jstring, NewStringUTF, (const char* utf), (override));
  MOCK_METHOD(jweak, NewWeakGlobalRef, (jobject obj), (override));
  MOCK_METHOD(jobject, PopLocalFrame, (jobject result), (override));
  MOCK_METHOD(jint, PushLocalFrame, (jint capacity), (override));
  MOCK_METHOD(jint, RegisterNatives,
              (jclass clazz, const JNINativeMethod* methods, jint nMethods),
              (override));
  MOCK_METHOD(void, ReleaseBooleanArrayElements,
              (jbooleanArray array, jboolean* elems, jint mode), (override));
  MOCK_METHOD(void, ReleaseByteArrayElements,
              (jbyteArray array, jbyte* elems, jint mode), (override));
  MOCK_METHOD(void, ReleaseCharArrayElements,
              (jcharArray array, jchar* elems, jint mode), (override));
  MOCK_METHOD(void, ReleaseDoubleArrayElements,
              (jdoubleArray array, jdouble* elems, jint mode), (override));
  MOCK_METHOD(void, ReleaseFloatArrayElements,
              (jfloatArray array, jfloat* elems, jint mode), (override));
  MOCK_METHOD(void, ReleaseIntArrayElements,
              (jintArray array, jint* elems, jint mode), (override));
  MOCK_METHOD(void, ReleaseLongArrayElements,
              (jlongArray array, jlong* elems, jint mode), (override));
  MOCK_METHOD(void, ReleasePrimitiveArrayCritical,
              (jarray array, void* carray, jint mode), (override));
  MOCK_METHOD(void, ReleaseShortArrayElements,
              (jshortArray array, jshort* elems, jint mode), (override));
  MOCK_METHOD(void, ReleaseStringChars, (jstring str, const jchar* chars),
              (override));
  MOCK_METHOD(void, ReleaseStringCritical, (jstring str, const jchar* cstring),
              (override));
  MOCK_METHOD(void, ReleaseStringUTFChars, (jstring str, const char* chars),
              (override));
  MOCK_METHOD(void, SetBooleanArrayRegion,
              (jbooleanArray array, jsize start, jsize len,
               const jboolean* buf),
              (override));
  MOCK_METHOD(void, SetBooleanField,
              (jobject obj, jfieldID fieldID, jboolean val), (override));
  MOCK_METHOD(void, SetByteArrayRegion,
              (jbyteArray array, jsize start, jsize len, const jbyte* buf),
              (override));
  MOCK_METHOD(void, SetByteField, (jobject obj, jfieldID fieldID, jbyte val),
              (override));
  MOCK_METHOD(void, SetCharArrayRegion,
              (jcharArray array, jsize start, jsize len, const jchar* buf),
              (override));
  MOCK_METHOD(void, SetCharField, (jobject obj, jfieldID fieldID, jchar val),
              (override));
  MOCK_METHOD(void, SetDoubleArrayRegion,
              (jdoubleArray array, jsize start, jsize len, const jdouble* buf),
              (override));
  MOCK_METHOD(void, SetDoubleField,
              (jobject obj, jfieldID fieldID, jdouble val), (override));
  MOCK_METHOD(void, SetFloatArrayRegion,
              (jfloatArray array, jsize start, jsize len, const jfloat* buf),
              (override));
  MOCK_METHOD(void, SetFloatField, (jobject obj, jfieldID fieldID, jfloat val),
              (override));
  MOCK_METHOD(void, SetIntArrayRegion,
              (jintArray array, jsize start, jsize len, const jint* buf),
              (override));
  MOCK_METHOD(void, SetIntField, (jobject obj, jfieldID fieldID, jint val),
              (override));
  MOCK_METHOD(void, SetLongArrayRegion,
              (jlongArray array, jsize start, jsize len, const jlong* buf),
              (override));
  MOCK_METHOD(void, SetLongField, (jobject obj, jfieldID fieldID, jlong val),
              (override));
  MOCK_METHOD(void, SetObjectArrayElement,
              (jobjectArray array, jsize index, jobject val), (override));
  MOCK_METHOD(void, SetObjectField,
              (jobject obj, jfieldID fieldID, jobject val), (override));
  MOCK_METHOD(void, SetShortArrayRegion,
              (jshortArray array, jsize start, jsize len, const jshort* buf),
              (override));
  MOCK_METHOD(void, SetShortField, (jobject obj, jfieldID fieldID, jshort val),
              (override));
  MOCK_METHOD(void, SetStaticBooleanField,
              (jclass clazz, jfieldID fieldID, jboolean value), (override));
  MOCK_METHOD(void, SetStaticByteField,
              (jclass clazz, jfieldID fieldID, jbyte value), (override));
  MOCK_METHOD(void, SetStaticCharField,
              (jclass clazz, jfieldID fieldID, jchar value), (override));
  MOCK_METHOD(void, SetStaticDoubleField,
              (jclass clazz, jfieldID fieldID, jdouble value), (override));
  MOCK_METHOD(void, SetStaticFloatField,
              (jclass clazz, jfieldID fieldID, jfloat value), (override));
  MOCK_METHOD(void, SetStaticIntField,
              (jclass clazz, jfieldID fieldID, jint value), (override));
  MOCK_METHOD(void, SetStaticLongField,
              (jclass clazz, jfieldID fieldID, jlong value), (override));
  MOCK_METHOD(void, SetStaticObjectField,
              (jclass clazz, jfieldID fieldID, jobject value), (override));
  MOCK_METHOD(void, SetStaticShortField,
              (jclass clazz, jfieldID fieldID, jshort value), (override));
  MOCK_METHOD(jint, Throw, (jthrowable obj), (override));
  MOCK_METHOD(jint, ThrowNew, (jclass clazz, const char* msg), (override));
  MOCK_METHOD(jobject, ToReflectedField,
              (jclass cls, jfieldID fieldID, jboolean isStatic), (override));
  MOCK_METHOD(jobject, ToReflectedMethod,
              (jclass cls, jmethodID methodID, jboolean isStatic), (override));
  MOCK_METHOD(jint, UnregisterNatives, (jclass clazz), (override));
};

}  // namespace jni::test

#endif  // JNI_BIND_MOCK_JNI_ENV_H_
