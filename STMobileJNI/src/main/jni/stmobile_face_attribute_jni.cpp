//
// Created by lei wang on 2016/11/23.
//

#include <jni.h>
#include <android/log.h>
#include "prebuilt/include/st_mobile_face_attribute.h"
#include "utils.h"

#include<fcntl.h>

#define  LOG_TAG "STMobileFaceAttributeNative"

extern "C" {
JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileFaceAttributeNative_createInstance(JNIEnv * env, jobject obj, jstring modelpath);
	JNIEXPORT int JNICALL Java_com_sensetime_stmobile_STMobileFaceAttributeNative_detect(JNIEnv * env, jobject obj,
	    jbyteArray pInputImage, jint imageFormat, jint imageWidth, jint imageHeight, jobjectArray face106Array, jobjectArray arrAttributes);
	JNIEXPORT void JNICALL Java_com_sensetime_stmobile_STMobileFaceAttributeNative_destroyInstance(JNIEnv * env, jobject obj);
}

JNIEXPORT jint JNICALL Java_com_sensetime_stmobile_STMobileFaceAttributeNative_createInstance(JNIEnv * env, jobject obj, jstring modelpath)
{
    st_handle_t  ha_handle = NULL;
	if (modelpath == NULL) {
	    LOGE("model path is null");
	    return ST_E_INVALIDARG;
	}
    const char *modelpathChars = env->GetStringUTFChars(modelpath, 0);
    LOGI("-->> modelpath=%s", modelpathChars);
    int result = st_mobile_face_attribute_create(modelpathChars, &ha_handle);
    if(result != 0) {
        LOGE("create handle for face attribute failed");
        env->ReleaseStringUTFChars(modelpath, modelpathChars);
        return result;
    }
    setHandle(env, obj, ha_handle);
    env->ReleaseStringUTFChars(modelpath, modelpathChars);
    return result;
}

JNIEXPORT int JNICALL Java_com_sensetime_stmobile_STMobileFaceAttributeNative_detect(JNIEnv * env, jobject obj,
    jbyteArray pInputImage, jint imageFormat, jint imageWidth, jint imageHeight, jobjectArray face106Array, jobjectArray arrAttributes)
{
    LOGI("faceAttribute, the width is %d, the height is %d",imageWidth, imageHeight);
    st_handle_t handle = getHandle<st_handle_t>(env, obj);
    if(handle == NULL)
    {
        LOGE("handle is null");
        return ST_E_INVALIDARG;
    }

    if (pInputImage == NULL) {
        LOGE("input image is null");
        return ST_E_INVALIDARG;
    }

    if (face106Array == NULL) {
        LOGE("face information is null");
        return ST_E_INVALIDARG;
    }

    jbyte *srcdata = (jbyte*) (env->GetByteArrayElements(pInputImage, 0));
    int image_stride = getImageStride((st_pixel_format)imageFormat, imageWidth);

    st_mobile_106_t *p_mobile_106 = NULL;

    int len = env->GetArrayLength(face106Array);
    if (len > 0) {
        p_mobile_106 = new st_mobile_106_t[len];
        for (int i = 0; i < len; ++i) {
            jobject face106 = env->GetObjectArrayElement(face106Array, i);
            if (!convert2mobile_106(env, face106, p_mobile_106[i])) {
                   memset(&p_mobile_106[i], 0, sizeof(st_mobile_106_t));
               }
            env->DeleteLocalRef(face106);
        }
    }
    LOGE("before detect： format: %d, %x, w: %d, h: %d,  stride: %d, %x, len: %d", imageFormat, srcdata, imageWidth, imageHeight, image_stride, p_mobile_106, len);
    st_mobile_attribute_t *pAttrbutes = NULL;
    int result = st_mobile_face_attribute_detect(handle, (unsigned char *)srcdata, (st_pixel_format)imageFormat,
    imageWidth, imageHeight, image_stride, p_mobile_106, len, &pAttrbutes);
    env->ReleaseByteArrayElements(pInputImage, srcdata, 0);
    LOGE("result detect : %d", result);

    if (result == ST_OK) {
        if (arrAttributes == NULL) {
            LOGE("face attribute array is null, please allocate it on java");
        } else {
            for (int i = 0; i < len; ++i) {
                jobject attrObj = convert2FaceAttribute(env, pAttrbutes[i]);
                if (attrObj != NULL) {
                    env->SetObjectArrayElement(arrAttributes, i, attrObj);
                }
                env->DeleteLocalRef(attrObj);
            }
        }
    }

    if (p_mobile_106) {
        delete[] p_mobile_106;
        p_mobile_106 = NULL;
    }

    return result;
}


JNIEXPORT void JNICALL Java_com_sensetime_stmobile_STMobileFaceAttributeNative_destroyInstance(JNIEnv * env, jobject obj)
{
    st_handle_t handle = getHandle<st_handle_t>(env, obj);
	if(handle != NULL)
	{
	    LOGI(" face attribute destory");
	    st_mobile_face_attribute_destroy(handle);
	}
}