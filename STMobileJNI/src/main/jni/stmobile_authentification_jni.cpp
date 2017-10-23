#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "st_mobile_license.h"
#include "utils.h"

#include<fcntl.h>

#define  LOG_TAG    "STMobileAuthentificationNative"

extern "C" {
JNIEXPORT jstring JNICALL Java_com_sensetime_stmobile_STMobileAuthentificationNative_generateActiveCode(JNIEnv * env, jobject obj, jstring licensePath);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileAuthentificationNative_checkActiveCode(JNIEnv * env, jobject obj, jstring licensePath, jstring activationCode);
JNIEXPORT jstring JNICALL Java_com_sensetime_stmobile_STMobileAuthentificationNative_generateActiveCodeFromBuffer(JNIEnv * env, jobject obj, jstring licenseBuffer, jint licenseSize);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileAuthentificationNative_checkActiveCodeFromBuffer(JNIEnv * env, jobject obj, jstring licenseBuffer, jint licenseSize, jstring activationCode);
};

JNIEXPORT jstring JNICALL Java_com_sensetime_stmobile_STMobileAuthentificationNative_generateActiveCode(JNIEnv * env, jobject obj, jstring licensePath) {
    LOGI("-->> 111generateActiveCode: start genrate active code");
//    const char *targetProductName = env->GetStringUTFChars(productName, 0);
    const char *targetLicensePath = env->GetStringUTFChars(licensePath, 0);
    char * activationCode = new char[1024];
    memset(activationCode, 0, 1024);
    int len = 1024;
    //	jint *len = (jint*) (env->GetPrimitiveArrayCritical(activeCodeLen, 0));
    LOGI("-->> targetLicensePath=%x, targetActivationCode=%x, activeCodeLen=%x", targetLicensePath, activationCode, len);
    int res = st_mobile_generate_activecode(targetLicensePath, activationCode, &len);
	LOGI("-->> targetLicensePath=%s, targetActivationCode=%s",targetLicensePath, activationCode);
    LOGI("-->> generateActiveCode: res=%d",res);
    jstring targetActivationCode = env->NewStringUTF(activationCode);

    env->ReleaseStringUTFChars(licensePath, targetLicensePath);
    //	env->ReleasePrimitiveArrayCritical(activeCodeLen, len, 0);
    return targetActivationCode;
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileAuthentificationNative_checkActiveCode(JNIEnv * env, jobject obj, jstring licensePath, jstring activationCode) {
    LOGI("-->> checkActiveCode: start check active code");
//    const char *targetProductName = env->GetStringUTFChars(productName, 0);
    const char *targetLicensePath = env->GetStringUTFChars(licensePath, 0);
    const char *targetActivationCode = env->GetStringUTFChars(activationCode, 0);
    //	LOGI("-->> targetProductName=%s, targetLicensePath=%s, targetActivationCode=%s",targetProductName, targetLicensePath, targetActivationCode);
    int res = st_mobile_check_activecode(targetLicensePath, targetActivationCode);
    	LOGI("-->> checkActiveCode: res=%d",res);
    env->ReleaseStringUTFChars(licensePath, targetLicensePath);
    env->ReleaseStringUTFChars(activationCode, targetActivationCode);
    return res;
}

JNIEXPORT jstring JNICALL Java_com_sensetime_stmobile_STMobileAuthentificationNative_generateActiveCodeFromBuffer(JNIEnv * env, jobject obj, jstring licenseBuffer, jint licenseSize) {
    LOGI("-->> 222generateActiveCodeFromBuffer: start genrate active code");
    const char *targetLicenseBuffer = env->GetStringUTFChars(licenseBuffer, 0);
    char * activationCode = new char[1024];
    memset(activationCode, 0, 1024);
    int len = 1024;
    int res = st_mobile_generate_activecode_from_buffer(targetLicenseBuffer, licenseSize, activationCode, &len);
    LOGI("-->> targetLicenseBuffer=%s, license_size=%d, targetActivationCode=%s",targetLicenseBuffer, licenseSize, activationCode);
    LOGI("-->> generateActiveCode: res=%d",res);
    jstring targetActivationCode = env->NewStringUTF(activationCode);

    env->ReleaseStringUTFChars(licenseBuffer, targetLicenseBuffer);
    return targetActivationCode;
}
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileAuthentificationNative_checkActiveCodeFromBuffer(JNIEnv * env, jobject obj, jstring licenseBuffer, jint licenseSize, jstring activationCode) {
    LOGI("-->> checkActiveCodeFromBuffer: start check active code");
    const char *targetLicenseBuffer = env->GetStringUTFChars(licenseBuffer, 0);
    const char *targetActiveCode = env->GetStringUTFChars(activationCode, 0);
//    int license_size = licenseSize;
    int res = st_mobile_check_activecode_from_buffer(targetLicenseBuffer, licenseSize, targetActiveCode);
    LOGI("-->> checkActiveCodeFromBuffer: res=%d",res);

    env->ReleaseStringUTFChars(licenseBuffer, targetLicenseBuffer);
    env->ReleaseStringUTFChars(activationCode, targetActiveCode);
    return res;
}
