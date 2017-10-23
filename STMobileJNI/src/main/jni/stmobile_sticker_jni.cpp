#include <jni.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "st_mobile_license.h"
#include "st_mobile_sticker.h"
#include "utils.h"

#include<fcntl.h>

#define  LOG_TAG    "STMobileSticker"


extern "C" {
JNIEXPORT jstring JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_generateActiveCode(JNIEnv * env, jobject obj, jstring licensePath);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_checkActiveCode(JNIEnv * env, jobject obj, jstring licensePath, jstring activationCode);
JNIEXPORT jstring JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_generateActiveCodeFromBuffer(JNIEnv * env, jobject obj, jstring licenseBuffer, jint licenseSize);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_checkActiveCodeFromBuffer(JNIEnv * env, jobject obj, jstring licenseBuffer, jint licenseSize, jstring activationCode);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_createInstance(JNIEnv * env, jobject obj, jstring zippath);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_processTexture(JNIEnv * env, jobject obj, jint textureIn, jobject humanAction, jint rotate, jint imageWidth, jint imageHeight, jboolean needsMirroring, jint textureOut);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_processTextureAndOutputBuffer(JNIEnv * env, jobject obj,jint textureIn, jobject humanAction, jint rotate, jint imageWidth, jint imageHeight, jboolean needsMirroring, jint textureOut, jint outFmt, jbyteArray imageOut);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_changeSticker(JNIEnv * env, jobject obj, jstring path);
JNIEXPORT void JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_destroyInstance(JNIEnv * env, jobject obj);
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_getTriggerAction(JNIEnv * env, jobject obj);
};

jfieldID    sticker_handle;
jfieldID    humanaction_handle;

static JavaVM *gJavaVM;
static jobject gInterfaceObject;
const char *kInterfacePath = "com/sensetime/stmobile/STMobileStickerNative";


void item_callback(const char* material_name, st_material_status statusCode) {
    LOGI("-->> item_callback: start item callback");

    int status;
    JNIEnv *env;
    bool isAttached = false;
    status = gJavaVM->AttachCurrentThread(&env, NULL);
    if(status<0) {
        LOGE("-->> item_callback: failed to attach current thread!");
        return;
    }
    isAttached = true;

    jclass interfaceClass = env->GetObjectClass(gInterfaceObject);
    if(!interfaceClass) {
        LOGE("-->> item_callback: failed to get class reference");
        if(isAttached) gJavaVM->DetachCurrentThread();
        return;
    }

    /* Find the callBack method ID */
    jmethodID method = env->GetStaticMethodID(interfaceClass, "item_callback", "(Ljava/lang/String;I)V");
    if(!method) {
        LOGE("item_callback: failed to get method ID");
        if(isAttached) gJavaVM->DetachCurrentThread();
        return;
    }

    jstring resultStr = env->NewStringUTF(material_name);
    LOGI("-->> item_callback: resultStr=%s, status=%d",material_name, statusCode);

    //get callback datas

    env->CallStaticVoidMethod(interfaceClass, method, resultStr, (jint)statusCode);
//	env->ReleaseStringChars(resultStr, material_name);
    LOGI("-->> material_name , status_string =&&&&");
//	if(isAttached) gJavaVM->DetachCurrentThread();
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *env;
    gJavaVM = vm;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("Failed to get the environment using GetEnv()");
        return -1;
    }

    jclass cls = env->FindClass(kInterfacePath);
    if(!cls) {
        LOGE("initClassHelper: failed to get %s class reference", kInterfacePath);
        return -1;
    }
    jmethodID constr = env->GetMethodID(cls, "<init>", "()V");
    if(!constr) {
        LOGE("initClassHelper: failed to get %s constructor", kInterfacePath);
        return -1;
    }
    jobject obj = env->NewObject(cls, constr);
    if(!obj) {
        LOGE("initClassHelper: failed to create a %s object", kInterfacePath);
        return -1;
    }
    gInterfaceObject = env->NewGlobalRef(obj);

    return JNI_VERSION_1_4;
}

static inline jfieldID getStickerHandleField(JNIEnv *env, jobject obj)
{
jclass c = env->GetObjectClass(obj);
// J is the type signature for long:
return env->GetFieldID(c, "nativeStickerHandle", "J");
}

void setStickerHandle(JNIEnv *env, jobject obj, void * h)
{
jlong handle = reinterpret_cast<jlong>(h);
env->SetLongField(obj, getStickerHandleField(env, obj), handle);
}

void* getStickerHandle(JNIEnv *env, jobject obj)
{
jlong handle = env->GetLongField(obj, getStickerHandleField(env, obj));
return reinterpret_cast<void *>(handle);
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_createInstance(JNIEnv * env, jobject obj, jstring zippath) {
    LOGI("createInstance");
    st_handle_t  ha_handle = NULL;
    const char *zippathChars = NULL;
    if (zippath != NULL) {
        zippathChars = env->GetStringUTFChars(zippath, 0);
    }
    
    LOGI("-->> zippath=%s", zippathChars);

    st_handle_t sticker_handle = NULL;
    int result = st_mobile_sticker_create(zippathChars, &sticker_handle);
    if(result != 0)
    {
        LOGE("st_mobile_sticker_create failed");
        return result;
    }
    setStickerHandle(env, obj, sticker_handle);

    env->ReleaseStringUTFChars(zippath, zippathChars);
    return result;
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_processTexture(JNIEnv * env, jobject obj, jint textureIn, 
    jobject humanAction, jint rotate, jint imageWidth, jint imageHeight, jboolean needsMirroring, jint textureOut)
{
    LOGI("processTexture, the width is %d, the height is %d, the rotate is %d",imageWidth, imageHeight, rotate);
    int result = -10;

    st_handle_t stickerhandle = getStickerHandle(env, obj);

    if(stickerhandle == NULL)
    {
        LOGE("handle is null");
    }

    st_mobile_human_action_t human_action;
    if (!convert2human_action(env, humanAction, human_action)) {
        memset(&human_action, 0, sizeof(st_mobile_human_action_t));
    }

    long startTime = getCurrentTime();
    if(stickerhandle != NULL)
    {
        result  = st_mobile_sticker_process_texture(stickerhandle, textureIn, imageWidth, imageHeight,  (st_rotate_type)rotate,needsMirroring, &human_action, item_callback, textureOut);
        LOGI("-->>st_mobile_sticker_process_texture --- result is %d", result);
    }

    long afterStickerTime = getCurrentTime();
    LOGI("process sticker time is %ld", (afterStickerTime - startTime));
    //	env->ReleasePrimitiveArrayCritical(pInputImage, srcdata, 0);
    
    return result;
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_processTextureAndOutputBuffer(JNIEnv * env, jobject obj,
    jint textureIn, jobject humanAction, jint rotate, jint imageWidth, jint imageHeight, jboolean needsMirroring,
    jint textureOut, jint outFmt, jbyteArray imageOut) {
    LOGI("processTexture, the width is %d, the height is %d, the rotate is %d",imageWidth, imageHeight, rotate);
        int result = -10;

        st_handle_t stickerhandle = getStickerHandle(env, obj);

        if(stickerhandle == NULL)
        {
            LOGE("handle is null");
        }

         jbyte *dstdata = NULL;
         if(imageOut != NULL)
         {
             dstdata = (jbyte*)(env->GetByteArrayElements(imageOut, 0));
         }

        st_mobile_human_action_t human_action;
        if (!convert2human_action(env, humanAction, human_action)) {
            memset(&human_action, 0, sizeof(st_mobile_human_action_t));
        }

        long startTime = getCurrentTime();
        if(stickerhandle != NULL)
        {
            //result  = st_mobile_sticker_process_texture(stickerhandle, textureIn, imageWidth, imageHeight,  (st_rotate_type)rotate,needsMirroring, &human_action, item_callback, textureOut);
            result  = st_mobile_sticker_process_and_output_texture(stickerhandle,
                                                                    textureIn, imageWidth, imageHeight, (st_rotate_type)rotate,needsMirroring,
                                                                       &human_action, item_callback, textureOut, (unsigned char*)dstdata, (st_pixel_format)outFmt);
            LOGI("-->>st_mobile_sticker_process_and_output_texture --- result is %d", result);
        }

        long afterStickerTime = getCurrentTime();
        LOGI("process sticker time is %ld", (afterStickerTime - startTime));
        if(dstdata != NULL)
        {
            env->ReleaseByteArrayElements(imageOut, dstdata, 0);
        }

        return result;
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_changeSticker(JNIEnv * env, jobject obj, jstring path)
{
    int result = JNI_FALSE;
    st_handle_t stickerhandle = getStickerHandle(env, obj);
    const char *pathChars = NULL;
    if (path != NULL) {
        pathChars = env->GetStringUTFChars(path, 0);
    }
    
    if(stickerhandle != NULL)
    {
        result = st_mobile_sticker_change_package(stickerhandle,pathChars);
    }
    if (pathChars != NULL) {
        env->ReleaseStringUTFChars(path, pathChars);
    }
    
    return result;
}

JNIEXPORT void JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_destroyInstance(JNIEnv * env, jobject obj)
{
    st_handle_t stickerhandle = getStickerHandle(env, obj);
    if(stickerhandle != NULL)
    {
        LOGI(" sticker handle destory ");
        st_mobile_sticker_destroy(stickerhandle);
    }
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileStickerNative_getTriggerAction(JNIEnv * env, jobject obj)
{
    st_handle_t stickerhandle = getStickerHandle(env, obj);
    if(stickerhandle != NULL)
    {
        unsigned int action = -1;
        int result = st_mobile_sticker_get_trigger_action(stickerhandle, &action);
        if (result == ST_OK) {
            return action;
        }
    }

    return -1;
}
