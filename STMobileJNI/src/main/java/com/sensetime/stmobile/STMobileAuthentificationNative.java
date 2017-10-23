package com.sensetime.stmobile;

/**
 * Licence验证JNI定义
 */

public class STMobileAuthentificationNative {


    static {
        System.loadLibrary("st_mobile");
        System.loadLibrary("stmobile_jni");
    }

    /**
     * 根据授权文件生成激活码, 在使用新的license文件时使用
     *
     * @param licensePath license文件路径
     * @return 成功返回0，错误返回其他，参考STCommon.ResultCode
     */
    public static native String generateActiveCode(String licensePath);

    /**
     * 检查激活码, 必须在所有接口之前调用
     *
     * @param licensePath    license文件路径
     * @param activationCode 当前设备的激活码
     * @return 成功返回0，错误返回其他，参考STCommon.ResultCode
     */
    public static native int checkActiveCode(String licensePath, String activationCode);


    /**
     * 根据授权文件缓存生成激活码, 在使用新的license文件时调用
     *
     * @param licenseBuffer license文件缓存
     * @param licenseSize   license文件缓存大小
     * @return 返回当前设备的激活码
     */
    public static native String generateActiveCodeFromBuffer(String licenseBuffer, int licenseSize);

    /**
     * 检查激活码, 必须在所有接口之前调用
     *
     * @param licenseBuffer  license文件缓存
     * @param licenseSize    license文件缓存大小
     * @param activationCode 当前设备的激活码
     * @return 成功返回0，错误返回其他，参考STCommon.ResultCode
     */
    public static native int checkActiveCodeFromBuffer(String licenseBuffer, int licenseSize, String activationCode);

}
