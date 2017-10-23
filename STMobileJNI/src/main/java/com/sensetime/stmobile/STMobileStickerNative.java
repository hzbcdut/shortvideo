package com.sensetime.stmobile;

import android.R.integer;

/**
 * 贴纸渲染JNI类定义
 */
public class STMobileStickerNative {

    //定义trigger action
    public final static int ST_MOBILE_EYE_BLINK = 0x00000002;    ///<  眨眼
    public final static int ST_MOBILE_MOUTH_AH = 0x00000004;    ///<  嘴巴大张
    public final static int ST_MOBILE_HEAD_YAW = 0x00000008;    ///<  摇头
    public final static int ST_MOBILE_HEAD_PITCH = 0x00000010;    ///<  点头
    public final static int ST_MOBILE_BROW_JUMP = 0x00000020;    ///<  眉毛挑动

    enum RenderStatus {
        ST_MATERIAL_BEGIN_RENDER(0), // 开始渲染子素材
        ST_MATERIAL_RENDERING(1),  // 子素材渲染中
        ST_MATERIAL_NO_RENDERING(2); // 子素材不再渲染

        private final int status;

        private RenderStatus(int status) {
            this.status = status;
        }

        public int getStatus() {
            return status;
        }

        public static RenderStatus fromStatus(int status) {
            for (RenderStatus type : RenderStatus.values()) {
                if (type.getStatus() == status) {
                    return type;
                }
            }
            return null;
        }
    }

    /**
     * 定义素材处理callback
     */
    public interface ItemCallback {
        /**
         * @param materialName 子素材名
         * @param status       当前子素材渲染的状态 RenderStatus
         */
        void processTextureCallback(String materialName, RenderStatus status);
    }

    private final static String TAG = STMobileStickerNative.class.getSimpleName();
    private static ItemCallback mCallback;

    /**
     * 设置要监听的素材处理callback
     *
     * @param callback 素材处理callback
     */
    public static void setCallback(ItemCallback callback) {
        mCallback = callback;
    }

    /**
     * JNI处理素材时，会回调该函数。
     */
    public static void item_callback(String materialName, int status) {
        if (mCallback != null) {
            mCallback.processTextureCallback(materialName, RenderStatus.fromStatus(status));
        }
    }

    static {
        System.loadLibrary("st_mobile");
        System.loadLibrary("stmobile_jni");
    }

    private long nativeStickerHandle;

    /**
     * 创建贴纸实例
     *
     * @param zipPath 输入的素材包路径,可以为空
     * @return 成功返回0，错误返回其他，参考STCommon.ResultCode
     */
    public native int createInstance(String zipPath);


    /**
     * 对OpenGL ES 中的纹理进行贴纸处理，必须在opengl环境中运行，仅支持RGBA图像格式
     *
     * @param textureIn      输入textureid
     * @param humanAction    输入检测到的人脸信息，由STMobileHumanActionNative相关的API获得
     * @param rotate         为使人脸正向，pInputImage需要旋转的角度。比如STRotateType.ST_CLOCKWISE_ROTATE_90
     * @param imageWidth     图像宽度（以像素为单位）
     * @param imageHeight    图像高度（以像素为单位）
     * @param needsMirroring 传入图像与显示图像是否是镜像关系
     * @param textureOut     处理后的纹理ID，用来做渲染
     * @return 成功返回0，错误返回其他，参考STCommon.ResultCode
     */
    public native int processTexture(int textureIn, STHumanAction humanAction, int rotate, int imageWidth, int imageHeight, boolean needsMirroring, int textureOut);

    /**
     * 对OpenGL ES 中的纹理进行贴纸处理，必须在opengl环境中运行，仅支持RGBA图像格式.支持buffer输出
     *
     * @param textureIn      输入textureid
     * @param humanAction    输入检测到的人脸信息，由STMobileHumanActionNative相关的API获得
     * @param rotate         为使人脸正向，pInputImage需要旋转的角度。比如STRotateType.ST_CLOCKWISE_ROTATE_90
     * @param imageWidth     图像宽度（以像素为单位）
     * @param imageHeight    图像高度（以像素为单位）
     * @param needsMirroring 传入图像与显示图像是否是镜像关系
     * @param textureOut     处理后的纹理ID，用来做渲染
     * @param outFmt         输出图像的格式，支持NV21,BGR,BGRA,NV12,RGBA等,比如STCommon.ST_PIX_FMT_NV12。
     * @param imageOut       输出图像的buffer，需要从外部创建
     * @return 成功返回0，错误返回其他，参考STCommon.ResultCode
     */
    public native int processTextureAndOutputBuffer(int textureIn, STHumanAction humanAction, int rotate, int imageWidth, int imageHeight, boolean needsMirroring, int textureOut, int outFmt, byte[] imageOut);

    /**
     * 切换贴纸路径
     *
     * @param path 贴纸路径。如果输入null，为无贴纸模式
     * @return 成功返回0，错误返回其他，参考STCommon.ResultCode
     */
    public native int changeSticker(String path);

    /**
     * 获取当前贴纸的触发动作
     *
     * @return 触发动作，比如STMobileHumanActionNative.ST_MOBILE_EYE_BLINK,
     * 比如STMobileHumanActionNative.ST_MOBILE_BROW_JUMP等
     */
    public native int getTriggerAction();


    /**
     * 销毁实例，必须在opengl环境中运行
     */
    public native void destroyInstance();
}
