#include <jni.h>

#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "prebuilt/include/st_mobile_common.h"
#include "prebuilt/include/st_mobile_sticker.h"
#include "utils.h"

#include<fcntl.h>

#define  LOG_TAG    "STMobileHumanAction"

extern "C" {
	JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileHumanActionNative_createInstance(JNIEnv * env, jobject obj, jstring modelpath, jint config);
	JNIEXPORT jobject JNICALL Java_com_sensetime_stmobile_STMobileHumanActionNative_humanActionDetect(JNIEnv * env, jobject obj,
	    jbyteArray pInputImage, jint imageFormat, jint detect_config, jint rotate, jint imageWidth, jint imageHeight);
	JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileHumanActionNative_reset(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_sensetime_stmobile_STMobileHumanActionNative_destroyInstance(JNIEnv * env, jobject obj);
};

static inline jfieldID getHumanActionHandleField(JNIEnv *env, jobject obj)
{
	jclass c = env->GetObjectClass(obj);
	// J is the type signature for long:
	return env->GetFieldID(c, "nativeHumanActionHandle", "J");
}

void setHumanActionHandle(JNIEnv *env, jobject obj, void * h)
{
	jlong handle = reinterpret_cast<jlong>(h);
	env->SetLongField(obj, getHumanActionHandleField(env, obj), handle);
}

void* getHumanActionHandle(JNIEnv *env, jobject obj)
{
	jlong handle = env->GetLongField(obj, getHumanActionHandleField(env, obj));
	return reinterpret_cast<void *>(handle);
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileHumanActionNative_createInstance(JNIEnv * env, jobject obj, jstring modelpath, jint config)
{
	st_handle_t  ha_handle = NULL;
	if (modelpath == NULL) {
	    LOGE("model path is null");
	    return ST_E_INVALIDARG;
	}
    const char *modelpathChars = env->GetStringUTFChars(modelpath, 0);
    LOGI("-->> modelpath=%s, config=%d", modelpathChars, config);
    int result = st_mobile_human_action_create(modelpathChars, config, &ha_handle);
    if(result != 0){
        LOGE("create handle for human action failed");
        env->ReleaseStringUTFChars(modelpath, modelpathChars);
        return result;
    }
    setHumanActionHandle(env, obj, ha_handle);
    env->ReleaseStringUTFChars(modelpath, modelpathChars);
    return result;
}
JNIEXPORT jobject JNICALL Java_com_sensetime_stmobile_STMobileHumanActionNative_humanActionDetect(JNIEnv * env, jobject obj, jbyteArray pInputImage, jint imageFormat,
jint detect_config, jint rotate, jint imageWidth, jint imageHeight)
{
	LOGI("humanActionDetect, the width is %d, the height is %d, the rotate is %d",imageWidth, imageHeight, rotate);
    st_handle_t humanActionhandle = getHumanActionHandle(env, obj);
    if(humanActionhandle == NULL)
    {
        LOGE("handle is null");
        return NULL;
    }

    if (pInputImage == NULL) {
        LOGE("input image is null");
        return NULL;
    }

    jbyte *srcdata = (jbyte*) (env->GetByteArrayElements(pInputImage, 0));
    int image_stride = getImageStride((st_pixel_format)imageFormat, imageWidth);

    st_mobile_human_action_t human_action;

    int result = -1;
    long startTime = getCurrentTime();
    if(humanActionhandle != NULL)
    {
        LOGE("before detect");
        result =  st_mobile_human_action_detect(humanActionhandle, (unsigned char *)srcdata,  (st_pixel_format)imageFormat,  imageWidth,
                                            imageHeight, image_stride, (st_rotate_type)rotate, detect_config, &human_action);
        LOGE("st_mobile_human_action_detect --- result is %d", result);
    }

    long afterdetectTime = getCurrentTime();
    LOGI("the human action detected time is %ld", (afterdetectTime - startTime));
    LOGI("the face count is %d", human_action.face_count);
    env->ReleaseByteArrayElements(pInputImage, srcdata, 0);

    jobject humanAction = NULL;
    if (result == ST_OK)
    {
        humanAction = convert2HumanAction(env, human_action);
    }

    return humanAction;
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileHumanActionNative_reset(JNIEnv * env, jobject obj)
{
	st_handle_t humanActionhandle = getHumanActionHandle(env, obj);
	if(humanActionhandle != NULL)
	{
	    return st_mobile_human_action_reset(humanActionhandle);
	}

	return ST_E_HANDLE;
}
JNIEXPORT void JNICALL Java_com_sensetime_stmobile_STMobileHumanActionNative_destroyInstance(JNIEnv * env, jobject obj)
{
	st_handle_t humanActionhandle = getHumanActionHandle(env, obj);
	if(humanActionhandle != NULL)
	{
	    LOGI(" human action destory");
	    st_mobile_human_action_destroy(humanActionhandle);
	}
}
