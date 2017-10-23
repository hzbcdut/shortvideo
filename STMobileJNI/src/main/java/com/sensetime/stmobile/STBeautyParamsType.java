package com.sensetime.stmobile;

/**
 * 定义可以美颜的类型
 */
public class STBeautyParamsType {
    public final static int ST_BEAUTIFY_CONTRAST_STRENGTH = 1; // 对比度强度，[0,1.0],缺省值为0.55
    public final static int ST_BEAUTIFY_SMOOTH_STRENGTH = 3; // 平滑强度， [0,1.0],缺省值为0.5492
    public final static int ST_BEAUTIFY_WHITEN_STRENGTH = 4;    /// 美白强度, [0,1.0], 缺省值为1
    public final static int ST_BEAUTIFY_ENLARGE_EYE_RATIO = 5;    /// 大眼比例, [0,1.0], 0.0不做大眼效果
    public final static int ST_BEAUTIFY_SHRINK_FACE_RATIO = 6;    /// 瘦脸比例, [0,1.0], 0.0不做瘦脸效果
    public final static int ST_BEAUTIFY_SHRINK_JAW_RATIO = 7;     /// 缩下巴比例, [0,1.0], 0.0不做缩下巴效果
}
