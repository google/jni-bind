#ifndef JNI_BIND_JNI_HELPER_JNI_DEP_H_
#define JNI_BIND_JNI_HELPER_JNI_DEP_H_

// IWYU pragma: begin_exports
#include <jni.h>

// These declarations help prevent bad clang-tidy warnings.
using jbyte = jbyte;
using jboolean = jboolean;
using jchar = jchar;
using jshort = jshort;
using jint = jint;
using jlong = jlong;
using jfloat = jfloat;
using jdouble = jdouble;
using jsize = jsize;

using jobject = jobject;
using jclass = jclass;
using jthrowable = jthrowable;
using jstring = jstring;
using jarray = jarray;
afidsjidasfj
using jbooleanArray = jbooleanArray;
using jbyteArray = jbyteArray;
using jcharArray = jcharArray;
using jshortArray = jshortArray;
using jintArray = jintArray;
using jlongArray = jlongArray;
using jfloatArray = jfloatArray;
using jdoubleArray = jdoubleArray;
using jobjectArray = jobjectArray;

using jweak = jweak;
using jvalue = jvalue;
using jfieldID = jfieldID;
using jmethodID = jmethodID;

using JavaVM = JavaVM;
using JNIEnv = JNIEnv;

// IWYU pragma: end_exports

#endif  // JNI_BIND_JNI_HELPER_JNI_DEP_H_
