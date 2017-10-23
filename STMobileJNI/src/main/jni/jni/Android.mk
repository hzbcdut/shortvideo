LOCAL_PATH := $(call my-dir)

LOCAL_PREBUILT_DIR := prebuilt

include $(CLEAR_VARS)
LOCAL_MODULE := libst_mobile
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_DIR)/lib/$(TARGET_ARCH_ABI)/libst_mobile.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := stmobile_jni
LOCAL_SRC_FILES := stmobile_beautify_jni.cpp\
                   stmobile_sticker_jni.cpp \
                   stmobile_humanaction_jni.cpp \
                   utils.cpp \
                   stmobile_authentification_jni.cpp \
                   stmobile_face_attribute_jni.cpp

LOCAL_LDLIBS := -llog -L$(SYSROOT)/usr/lib/$(TARGET_ARCH_ABI) -lz -ljnigraphics -lGLESv2

LOCAL_SHARED_LIBRARIES := libst_mobile
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/$(LOCAL_PREBUILT_DIR)/include

include $(BUILD_SHARED_LIBRARY)
