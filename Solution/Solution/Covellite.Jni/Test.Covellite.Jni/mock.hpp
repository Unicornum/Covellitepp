
#pragma once

/**
* \file
*  ќбъ€влени€ подставных классов.
*/

#include "Mock\Namespaces.hpp"

using jint = int;
class _jobject {};
using jobject = _jobject *;
class _jclass {};
using jclass = _jclass *;
struct _jmethodID;
using jmethodID  = _jmethodID *;
struct _jfieldID;
using jfieldID = _jfieldID *;

class JNIEnv 
{
public:
  jclass FindClass(const char *) { return nullptr; }
  jclass GetObjectClass(jobject) { return nullptr; }
  jmethodID GetMethodID(jclass, const char *, const char *) { return nullptr; }

public:
  jfieldID GetStaticFieldID(jclass, const char *, const char *) { return nullptr; }
  jfieldID GetFieldID(jclass, const char *, const char *) { return nullptr; }
  jobject GetStaticObjectField(jclass, jfieldID) { return nullptr; }
  jint GetIntField(jobject, jfieldID) { return 0; }

public:
  jobject NewObject(jclass, jmethodID, ...) { return nullptr; }
  void CallVoidMethod(jobject, jmethodID, ...) {}
  jobject CallObjectMethod(jobject, jmethodID) { return nullptr; }
  jobject CallObjectMethod(jobject, jmethodID, jobject) { return nullptr; }
  jobject CallBooleanMethod(jobject, jmethodID, jobject, jint) { return nullptr; }
};

class JavaVM {};

#include <alicorn\platform\environment.mock.hpp>
