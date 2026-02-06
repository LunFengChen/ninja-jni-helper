/*
 * JNI Fallback Type Definitions
 * 
 * Complete JNI 1.6 type definitions for Binary Ninja fallback injection.
 * This file is used when the type library (.bntl) fails to load.
 * 
 * Based on: jni.h from Android NDK
 */

/* Basic types */
typedef uint8_t jboolean;
typedef int8_t jbyte;
typedef uint16_t jchar;
typedef int16_t jshort;
typedef int32_t jint;
typedef int64_t jlong;
typedef float jfloat;
typedef double jdouble;
typedef jint jsize;

/* Opaque reference types */
struct _jobject {};
struct _jclass {};
struct _jstring {};
struct _jarray {};
struct _jthrowable {};
struct _jfieldID {};
struct _jmethodID {};

typedef struct _jobject* jobject;
typedef struct _jclass* jclass;
typedef struct _jstring* jstring;
typedef struct _jarray* jarray;
typedef struct _jthrowable* jthrowable;
typedef struct _jfieldID* jfieldID;
typedef struct _jmethodID* jmethodID;

typedef jobject jweak;
typedef jobject jobjectArray;
typedef jarray jbooleanArray;
typedef jarray jbyteArray;
typedef jarray jcharArray;
typedef jarray jshortArray;
typedef jarray jintArray;
typedef jarray jlongArray;
typedef jarray jfloatArray;
typedef jarray jdoubleArray;

/* Union for method arguments */
typedef union jvalue {
    jboolean z;
    jbyte b;
    jchar c;
    jshort s;
    jint i;
    jlong j;
    jfloat f;
    jdouble d;
    jobject l;
} jvalue;

/*
 * JNINativeInterface - Complete function pointer table
 * 
 * This structure allows Binary Ninja to recognize JNI API calls
 * through function pointer offsets (e.g., env->NewStringUTF at offset 0x538)
 */
struct JNINativeInterface {
    void* reserved0;
    void* reserved1;
    void* reserved2;
    void* reserved3;
    
    jint (*GetVersion)(void*);
    jclass (*DefineClass)(void*, const char*, jobject, const jbyte*, jsize);
    jclass (*FindClass)(void*, const char*);
    
    jmethodID (*FromReflectedMethod)(void*, jobject);
    jfieldID (*FromReflectedField)(void*, jobject);
    jobject (*ToReflectedMethod)(void*, jclass, jmethodID, jboolean);
    
    jclass (*GetSuperclass)(void*, jclass);
    jboolean (*IsAssignableFrom)(void*, jclass, jclass);
    jobject (*ToReflectedField)(void*, jclass, jfieldID, jboolean);
    
    jint (*Throw)(void*, jthrowable);
    jint (*ThrowNew)(void*, jclass, const char*);
    jthrowable (*ExceptionOccurred)(void*);
    void (*ExceptionDescribe)(void*);
    void (*ExceptionClear)(void*);
    void (*FatalError)(void*, const char*);
    
    jint (*PushLocalFrame)(void*, jint);
    jobject (*PopLocalFrame)(void*, jobject);
    
    jobject (*NewGlobalRef)(void*, jobject);
    void (*DeleteGlobalRef)(void*, jobject);
    void (*DeleteLocalRef)(void*, jobject);
    jboolean (*IsSameObject)(void*, jobject, jobject);
    jobject (*NewLocalRef)(void*, jobject);
    jint (*EnsureLocalCapacity)(void*, jint);
    
    jobject (*AllocObject)(void*, jclass);
    jobject (*NewObject)(void*, jclass, jmethodID, ...);
    jobject (*NewObjectV)(void*, jclass, jmethodID, va_list);
    jobject (*NewObjectA)(void*, jclass, jmethodID, const jvalue*);
    
    jclass (*GetObjectClass)(void*, jobject);
    jboolean (*IsInstanceOf)(void*, jobject, jclass);
    
    jmethodID (*GetMethodID)(void*, jclass, const char*, const char*);
    
    jobject (*CallObjectMethod)(void*, jobject, jmethodID, ...);
    jobject (*CallObjectMethodV)(void*, jobject, jmethodID, va_list);
    jobject (*CallObjectMethodA)(void*, jobject, jmethodID, const jvalue*);
    jboolean (*CallBooleanMethod)(void*, jobject, jmethodID, ...);
    jboolean (*CallBooleanMethodV)(void*, jobject, jmethodID, va_list);
    jboolean (*CallBooleanMethodA)(void*, jobject, jmethodID, const jvalue*);
    jbyte (*CallByteMethod)(void*, jobject, jmethodID, ...);
    jbyte (*CallByteMethodV)(void*, jobject, jmethodID, va_list);
    jbyte (*CallByteMethodA)(void*, jobject, jmethodID, const jvalue*);
    jchar (*CallCharMethod)(void*, jobject, jmethodID, ...);
    jchar (*CallCharMethodV)(void*, jobject, jmethodID, va_list);
    jchar (*CallCharMethodA)(void*, jobject, jmethodID, const jvalue*);
    jshort (*CallShortMethod)(void*, jobject, jmethodID, ...);
    jshort (*CallShortMethodV)(void*, jobject, jmethodID, va_list);
    jshort (*CallShortMethodA)(void*, jobject, jmethodID, const jvalue*);
    jint (*CallIntMethod)(void*, jobject, jmethodID, ...);
    jint (*CallIntMethodV)(void*, jobject, jmethodID, va_list);
    jint (*CallIntMethodA)(void*, jobject, jmethodID, const jvalue*);
    jlong (*CallLongMethod)(void*, jobject, jmethodID, ...);
    jlong (*CallLongMethodV)(void*, jobject, jmethodID, va_list);
    jlong (*CallLongMethodA)(void*, jobject, jmethodID, const jvalue*);
    jfloat (*CallFloatMethod)(void*, jobject, jmethodID, ...);
    jfloat (*CallFloatMethodV)(void*, jobject, jmethodID, va_list);
    jfloat (*CallFloatMethodA)(void*, jobject, jmethodID, const jvalue*);
    jdouble (*CallDoubleMethod)(void*, jobject, jmethodID, ...);
    jdouble (*CallDoubleMethodV)(void*, jobject, jmethodID, va_list);
    jdouble (*CallDoubleMethodA)(void*, jobject, jmethodID, const jvalue*);
    void (*CallVoidMethod)(void*, jobject, jmethodID, ...);
    void (*CallVoidMethodV)(void*, jobject, jmethodID, va_list);
    void (*CallVoidMethodA)(void*, jobject, jmethodID, const jvalue*);
    
    jobject (*CallNonvirtualObjectMethod)(void*, jobject, jclass, jmethodID, ...);
    jobject (*CallNonvirtualObjectMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jobject (*CallNonvirtualObjectMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    jboolean (*CallNonvirtualBooleanMethod)(void*, jobject, jclass, jmethodID, ...);
    jboolean (*CallNonvirtualBooleanMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jboolean (*CallNonvirtualBooleanMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    jbyte (*CallNonvirtualByteMethod)(void*, jobject, jclass, jmethodID, ...);
    jbyte (*CallNonvirtualByteMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jbyte (*CallNonvirtualByteMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    jchar (*CallNonvirtualCharMethod)(void*, jobject, jclass, jmethodID, ...);
    jchar (*CallNonvirtualCharMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jchar (*CallNonvirtualCharMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    jshort (*CallNonvirtualShortMethod)(void*, jobject, jclass, jmethodID, ...);
    jshort (*CallNonvirtualShortMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jshort (*CallNonvirtualShortMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    jint (*CallNonvirtualIntMethod)(void*, jobject, jclass, jmethodID, ...);
    jint (*CallNonvirtualIntMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jint (*CallNonvirtualIntMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    jlong (*CallNonvirtualLongMethod)(void*, jobject, jclass, jmethodID, ...);
    jlong (*CallNonvirtualLongMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jlong (*CallNonvirtualLongMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    jfloat (*CallNonvirtualFloatMethod)(void*, jobject, jclass, jmethodID, ...);
    jfloat (*CallNonvirtualFloatMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jfloat (*CallNonvirtualFloatMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    jdouble (*CallNonvirtualDoubleMethod)(void*, jobject, jclass, jmethodID, ...);
    jdouble (*CallNonvirtualDoubleMethodV)(void*, jobject, jclass, jmethodID, va_list);
    jdouble (*CallNonvirtualDoubleMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    void (*CallNonvirtualVoidMethod)(void*, jobject, jclass, jmethodID, ...);
    void (*CallNonvirtualVoidMethodV)(void*, jobject, jclass, jmethodID, va_list);
    void (*CallNonvirtualVoidMethodA)(void*, jobject, jclass, jmethodID, const jvalue*);
    
    jfieldID (*GetFieldID)(void*, jclass, const char*, const char*);
    
    jobject (*GetObjectField)(void*, jobject, jfieldID);
    jboolean (*GetBooleanField)(void*, jobject, jfieldID);
    jbyte (*GetByteField)(void*, jobject, jfieldID);
    jchar (*GetCharField)(void*, jobject, jfieldID);
    jshort (*GetShortField)(void*, jobject, jfieldID);
    jint (*GetIntField)(void*, jobject, jfieldID);
    jlong (*GetLongField)(void*, jobject, jfieldID);
    jfloat (*GetFloatField)(void*, jobject, jfieldID);
    jdouble (*GetDoubleField)(void*, jobject, jfieldID);
    
    void (*SetObjectField)(void*, jobject, jfieldID, jobject);
    void (*SetBooleanField)(void*, jobject, jfieldID, jboolean);
    void (*SetByteField)(void*, jobject, jfieldID, jbyte);
    void (*SetCharField)(void*, jobject, jfieldID, jchar);
    void (*SetShortField)(void*, jobject, jfieldID, jshort);
    void (*SetIntField)(void*, jobject, jfieldID, jint);
    void (*SetLongField)(void*, jobject, jfieldID, jlong);
    void (*SetFloatField)(void*, jobject, jfieldID, jfloat);
    void (*SetDoubleField)(void*, jobject, jfieldID, jdouble);
    
    jmethodID (*GetStaticMethodID)(void*, jclass, const char*, const char*);
    
    jobject (*CallStaticObjectMethod)(void*, jclass, jmethodID, ...);
    jobject (*CallStaticObjectMethodV)(void*, jclass, jmethodID, va_list);
    jobject (*CallStaticObjectMethodA)(void*, jclass, jmethodID, const jvalue*);
    jboolean (*CallStaticBooleanMethod)(void*, jclass, jmethodID, ...);
    jboolean (*CallStaticBooleanMethodV)(void*, jclass, jmethodID, va_list);
    jboolean (*CallStaticBooleanMethodA)(void*, jclass, jmethodID, const jvalue*);
    jbyte (*CallStaticByteMethod)(void*, jclass, jmethodID, ...);
    jbyte (*CallStaticByteMethodV)(void*, jclass, jmethodID, va_list);
    jbyte (*CallStaticByteMethodA)(void*, jclass, jmethodID, const jvalue*);
    jchar (*CallStaticCharMethod)(void*, jclass, jmethodID, ...);
    jchar (*CallStaticCharMethodV)(void*, jclass, jmethodID, va_list);
    jchar (*CallStaticCharMethodA)(void*, jclass, jmethodID, const jvalue*);
    jshort (*CallStaticShortMethod)(void*, jclass, jmethodID, ...);
    jshort (*CallStaticShortMethodV)(void*, jclass, jmethodID, va_list);
    jshort (*CallStaticShortMethodA)(void*, jclass, jmethodID, const jvalue*);
    jint (*CallStaticIntMethod)(void*, jclass, jmethodID, ...);
    jint (*CallStaticIntMethodV)(void*, jclass, jmethodID, va_list);
    jint (*CallStaticIntMethodA)(void*, jclass, jmethodID, const jvalue*);
    jlong (*CallStaticLongMethod)(void*, jclass, jmethodID, ...);
    jlong (*CallStaticLongMethodV)(void*, jclass, jmethodID, va_list);
    jlong (*CallStaticLongMethodA)(void*, jclass, jmethodID, const jvalue*);
    jfloat (*CallStaticFloatMethod)(void*, jclass, jmethodID, ...);
    jfloat (*CallStaticFloatMethodV)(void*, jclass, jmethodID, va_list);
    jfloat (*CallStaticFloatMethodA)(void*, jclass, jmethodID, const jvalue*);
    jdouble (*CallStaticDoubleMethod)(void*, jclass, jmethodID, ...);
    jdouble (*CallStaticDoubleMethodV)(void*, jclass, jmethodID, va_list);
    jdouble (*CallStaticDoubleMethodA)(void*, jclass, jmethodID, const jvalue*);
    void (*CallStaticVoidMethod)(void*, jclass, jmethodID, ...);
    void (*CallStaticVoidMethodV)(void*, jclass, jmethodID, va_list);
    void (*CallStaticVoidMethodA)(void*, jclass, jmethodID, const jvalue*);
    
    jfieldID (*GetStaticFieldID)(void*, jclass, const char*, const char*);
    jobject (*GetStaticObjectField)(void*, jclass, jfieldID);
    jboolean (*GetStaticBooleanField)(void*, jclass, jfieldID);
    jbyte (*GetStaticByteField)(void*, jclass, jfieldID);
    jchar (*GetStaticCharField)(void*, jclass, jfieldID);
    jshort (*GetStaticShortField)(void*, jclass, jfieldID);
    jint (*GetStaticIntField)(void*, jclass, jfieldID);
    jlong (*GetStaticLongField)(void*, jclass, jfieldID);
    jfloat (*GetStaticFloatField)(void*, jclass, jfieldID);
    jdouble (*GetStaticDoubleField)(void*, jclass, jfieldID);
    
    void (*SetStaticObjectField)(void*, jclass, jfieldID, jobject);
    void (*SetStaticBooleanField)(void*, jclass, jfieldID, jboolean);
    void (*SetStaticByteField)(void*, jclass, jfieldID, jbyte);
    void (*SetStaticCharField)(void*, jclass, jfieldID, jchar);
    void (*SetStaticShortField)(void*, jclass, jfieldID, jshort);
    void (*SetStaticIntField)(void*, jclass, jfieldID, jint);
    void (*SetStaticLongField)(void*, jclass, jfieldID, jlong);
    void (*SetStaticFloatField)(void*, jclass, jfieldID, jfloat);
    void (*SetStaticDoubleField)(void*, jclass, jfieldID, jdouble);
    
    jstring (*NewString)(void*, const jchar*, jsize);
    jsize (*GetStringLength)(void*, jstring);
    const jchar* (*GetStringChars)(void*, jstring, jboolean*);
    void (*ReleaseStringChars)(void*, jstring, const jchar*);
    
    jstring (*NewStringUTF)(void*, const char*);
    jsize (*GetStringUTFLength)(void*, jstring);
    const char* (*GetStringUTFChars)(void*, jstring, jboolean*);
    void (*ReleaseStringUTFChars)(void*, jstring, const char*);
    
    jsize (*GetArrayLength)(void*, jarray);
    
    jobjectArray (*NewObjectArray)(void*, jsize, jclass, jobject);
    jobject (*GetObjectArrayElement)(void*, jobjectArray, jsize);
    void (*SetObjectArrayElement)(void*, jobjectArray, jsize, jobject);
    
    jbooleanArray (*NewBooleanArray)(void*, jsize);
    jbyteArray (*NewByteArray)(void*, jsize);
    jcharArray (*NewCharArray)(void*, jsize);
    jshortArray (*NewShortArray)(void*, jsize);
    jintArray (*NewIntArray)(void*, jsize);
    jlongArray (*NewLongArray)(void*, jsize);
    jfloatArray (*NewFloatArray)(void*, jsize);
    jdoubleArray (*NewDoubleArray)(void*, jsize);
    
    jboolean* (*GetBooleanArrayElements)(void*, jbooleanArray, jboolean*);
    jbyte* (*GetByteArrayElements)(void*, jbyteArray, jboolean*);
    jchar* (*GetCharArrayElements)(void*, jcharArray, jboolean*);
    jshort* (*GetShortArrayElements)(void*, jshortArray, jboolean*);
    jint* (*GetIntArrayElements)(void*, jintArray, jboolean*);
    jlong* (*GetLongArrayElements)(void*, jlongArray, jboolean*);
    jfloat* (*GetFloatArrayElements)(void*, jfloatArray, jboolean*);
    jdouble* (*GetDoubleArrayElements)(void*, jdoubleArray, jboolean*);
    
    void (*ReleaseBooleanArrayElements)(void*, jbooleanArray, jboolean*, jint);
    void (*ReleaseByteArrayElements)(void*, jbyteArray, jbyte*, jint);
    void (*ReleaseCharArrayElements)(void*, jcharArray, jchar*, jint);
    void (*ReleaseShortArrayElements)(void*, jshortArray, jshort*, jint);
    void (*ReleaseIntArrayElements)(void*, jintArray, jint*, jint);
    void (*ReleaseLongArrayElements)(void*, jlongArray, jlong*, jint);
    void (*ReleaseFloatArrayElements)(void*, jfloatArray, jfloat*, jint);
    void (*ReleaseDoubleArrayElements)(void*, jdoubleArray, jdouble*, jint);
    
    void (*GetBooleanArrayRegion)(void*, jbooleanArray, jsize, jsize, jboolean*);
    void (*GetByteArrayRegion)(void*, jbyteArray, jsize, jsize, jbyte*);
    void (*GetCharArrayRegion)(void*, jcharArray, jsize, jsize, jchar*);
    void (*GetShortArrayRegion)(void*, jshortArray, jsize, jsize, jshort*);
    void (*GetIntArrayRegion)(void*, jintArray, jsize, jsize, jint*);
    void (*GetLongArrayRegion)(void*, jlongArray, jsize, jsize, jlong*);
    void (*GetFloatArrayRegion)(void*, jfloatArray, jsize, jsize, jfloat*);
    void (*GetDoubleArrayRegion)(void*, jdoubleArray, jsize, jsize, jdouble*);
    
    void (*SetBooleanArrayRegion)(void*, jbooleanArray, jsize, jsize, const jboolean*);
    void (*SetByteArrayRegion)(void*, jbyteArray, jsize, jsize, const jbyte*);
    void (*SetCharArrayRegion)(void*, jcharArray, jsize, jsize, const jchar*);
    void (*SetShortArrayRegion)(void*, jshortArray, jsize, jsize, const jshort*);
    void (*SetIntArrayRegion)(void*, jintArray, jsize, jsize, const jint*);
    void (*SetLongArrayRegion)(void*, jlongArray, jsize, jsize, const jlong*);
    void (*SetFloatArrayRegion)(void*, jfloatArray, jsize, jsize, const jfloat*);
    void (*SetDoubleArrayRegion)(void*, jdoubleArray, jsize, jsize, const jdouble*);
    
    jint (*RegisterNatives)(void*, jclass, const void*, jint);
    jint (*UnregisterNatives)(void*, jclass);
    
    jint (*MonitorEnter)(void*, jobject);
    jint (*MonitorExit)(void*, jobject);
    
    jint (*GetJavaVM)(void*, void**);
    
    void (*GetStringRegion)(void*, jstring, jsize, jsize, jchar*);
    void (*GetStringUTFRegion)(void*, jstring, jsize, jsize, char*);
    
    void* (*GetPrimitiveArrayCritical)(void*, jarray, jboolean*);
    void (*ReleasePrimitiveArrayCritical)(void*, jarray, void*, jint);
    
    const jchar* (*GetStringCritical)(void*, jstring, jboolean*);
    void (*ReleaseStringCritical)(void*, jstring, const jchar*);
    
    jweak (*NewWeakGlobalRef)(void*, jobject);
    void (*DeleteWeakGlobalRef)(void*, jweak);
    
    jboolean (*ExceptionCheck)(void*);
    
    jobject (*NewDirectByteBuffer)(void*, void*, jlong);
    void* (*GetDirectBufferAddress)(void*, jobject);
    jlong (*GetDirectBufferCapacity)(void*, jobject);
    
    int (*GetObjectRefType)(void*, jobject);
};

/* JNIEnv is a pointer to the function table */
typedef const struct JNINativeInterface* JNIEnv;

/* JavaVM placeholder */
typedef void* JavaVM;
