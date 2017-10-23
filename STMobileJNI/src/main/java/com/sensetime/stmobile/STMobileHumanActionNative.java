package com.sensetime.stmobile;

/**
 * 人脸关键点及人脸行为识别
 */
public class STMobileHumanActionNative {
    private final static String TAG = STMobileHumanActionNative.class.getSimpleName();

    //支持的人脸行为配置
    public final static int ST_MOBILE_FACE_DETECT = 0x00000001;    ///<  人脸检测

    //人脸动作
    public final static int ST_MOBILE_EYE_BLINK = 0x00000002;    ///<  眨眼
    public final static int ST_MOBILE_MOUTH_AH = 0x00000004;    ///<  嘴巴大张
    public final static int ST_MOBILE_HEAD_YAW = 0x00000008;    ///<  摇头
    public final static int ST_MOBILE_HEAD_PITCH = 0x00000010;    ///<  点头
    public final static int ST_MOBILE_BROW_JUMP = 0x00000020;    ///<  眉毛挑动
    public final static int ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_DETECT = 0x000000FF;  ///< 全部检测

    //创建时默认的配置参数
    // 使用多线程+320W，可最大限度的提高速度，并减少卡顿。默认检测face action，打开head_pose去抖动
    // 对视频进行检测推荐使用多线程检
    public final static int ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO = STCommon.ST_MOBILE_TRACKING_MULTI_THREAD
            | STCommon.ST_MOBILE_TRACKING_RESIZE_IMG_320W | STCommon.ST_MOBILE_TRACKING_ENABLE_FACE_ACTION
            | STCommon.ST_MOBILE_TRACKING_ENABLE_DEBOUNCE;
    // 对图片进行检测只能使用单线程
    public final static int ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_IMAGE = STCommon.ST_MOBILE_TRACKING_SINGLE_THREAD;

    static {
        System.loadLibrary("st_mobile");
        System.loadLibrary("stmobile_jni");
    }

    //供JNI使用，应用不需要关注
    private long nativeHumanActionHandle;

    /**
     * 创建实例
     *
     * @param modelpath 模型文件的,例如models/face_track_2.x.x.model
     * @param config    配置选项，比如STCommon.ST_MOBILE_TRACKING_RESIZE_IMG_320W。建议输入ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO
     * @return 成功返回0，错误返回其他，参考STCommon.ResultCode
     */
    public native int createInstance(String modelpath, int config);

    /**
     * 检测人脸106点及人脸行为
     *
     * @param imgData       用于检测的图像数据
     * @param imageFormat   用于检测的图像数据的像素格式,比如STCommon.ST_PIX_FMT_NV12。能够独立提取灰度通道的图像格式处理速度较快，
     *                      比如ST_PIX_FMT_GRAY8，ST_PIX_FMT_YUV420P，ST_PIX_FMT_NV12，ST_PIX_FMT_NV21
     * @param detect_config 检测选项，代表当前需要检测哪些动作，例如ST_MOBILE_EYE_BLINK|ST_MOBILE_MOUTH_AH表示当前帧只检测眨眼和张嘴
     * @param orientation   图像中人脸的方向,例如STRotateType.ST_CLOCKWISE_ROTATE_0
     * @param imageWidth    用于检测的图像的宽度(以像素为单位)
     * @param imageHeight   用于检测的图像的高度(以像素为单位)
     * @return 人脸信息
     */
    public native STHumanAction humanActionDetect(byte[] imgData, int imageFormat, int detect_config,
                                                  int orientation, int imageWidth, int imageHeight);

    /**
     * 重置，清除所有缓存信息
     */
    public native void reset();

    /**
     * 释放instance
     */
    public native void destroyInstance();
}
