#include "JavaApp.h"
#include "ImmServiceManager.h"
#include <string>
#include <iostream>

using namespace std;
jboolean Java_JavaApp_writeDataReport(JNIEnv *env, jobject obj, jstring cpuLoad, jstring ramUsage){

	const char *nativeCpu = env->GetStringUTFChars(cpuLoad, 0);
	const char *nativeRam = env->GetStringUTFChars(ramUsage, 0);
	string cpuStr(nativeCpu);
	string ramStr(nativeRam);
  	ImmServiceManager* immService = new ImmServiceManager();
  	return immService->writeDataReport(cpuStr,ramStr);
}


jboolean Java_JavaApp_readDataReport(JNIEnv *env, jobject obj){
  	ImmServiceManager* immService = new ImmServiceManager();
  	return immService->readDataReport();
}