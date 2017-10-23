#ifndef INCLUDE_STMOBILE_ST_MOBILE_BEAUTIFY_H_
#define INCLUDE_STMOBILE_ST_MOBILE_BEAUTIFY_H_

#include "st_mobile_common.h"

//>> CONFIG_ST_MOBILE_API_BEAUTIFY
/// @defgroup st_mobile_beautify
/// @brief beautify interfaces
///
/// This set of interfaces processing beautify routines
///
/// @{

/// @brief 创建美颜句柄
/// @param [in] image_width 图像宽度
/// @param [in] image_height  图像高度
/// @param [out] handle 初始化的美颜句柄
/// @return 成功返回ST_OK, 错误则返回错误码
ST_SDK_API st_result_t
st_mobile_beautify_create(
    int image_width, int image_height,
    st_handle_t *handle
);

///@brief 美颜参数类型
typedef enum {
	ST_BEAUTIFY_CONTRAST_STRENGTH = 1,	/// 对比度强度, [0,1.0]
	ST_BEAUTIFY_SMOOTH_STRENGTH = 3,	/// 磨皮强度, [0,1.0]
	ST_BEAUTIFY_WHITEN_STRENGTH = 4,	/// 美白强度, [0,1.0]
	ST_BEAUTIFY_ENLARGE_EYE_RATIO = 5,	/// 大眼比例, [0,1.0], 0.0不做大眼效果
	ST_BEAUTIFY_SHRINK_FACE_RATIO = 6, 	/// 瘦脸比例, [0,1.0], 0.0不做瘦脸效果
	ST_BEAUTIFY_SHRINK_JAW_RATIO = 7    /// 缩下巴比例, [0,1.0], 0.0不做缩下巴效果
} st_beautify_type;

/// @brief 设置美颜参数，默认包含所有功能参数和强度参数
/// @param [in] handle 已初始化的美颜句柄
/// @param [in] type 美颜参数关键字， 例如ST_BEAUTIFY_CONTRAST_STRENGTH、ST_BEAUTIFY_SMOOTH_STRENGTH
/// @param [in] value 参数取值
/// @return 成功返回ST_OK, 错误则返回错误码,错误码定义在st_mobile_common.h 中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_beautify_setparam(
	st_handle_t handle,
	st_beautify_type type,
	float value
);

/// @brief 对图像buffer做美颜处理
/// @param[in] handle 已初始化的美颜句柄
/// @param[in] img_in 输入图片的数据数组
/// @param[in] fmt_in 输入图片的类型,支持NV21,BGR,BGRA,NV12,RGBA格式。
/// @param[in] image_width 输入图片的宽度(以像素为单位)
/// @param[in] image_height 输入图片的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位)，即每行的字节数；目前仅支持字节对齐的padding，不支持roi
/// @param[in] p_faces_array_in 输入需要美颜的人脸106点数组；如果为NULL，不执行大眼瘦脸
/// @param[in] faces_count 需要美颜的人脸数目；如果为0，不执行大眼瘦脸
/// @param[out] img_out 输出图像数据数组
/// @param[in] fmt_out 输出图片的类型,支持NV21,BGR,BGRA,NV12,RGBA格式
/// @param[out] p_faces_array_out 输出美颜后的人脸106点数组，需要由用户分配内存，必须与输入的人脸106点数组大小相同；如果为NULL，不输出美颜后的人脸106点数组
/// @return 成功返回ST_OK, 错误则返回错误码，错误码定义在st_mobile_common.h 中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_beautify_process_buffer(
	st_handle_t handle,
	const unsigned char *img_in, st_pixel_format fmt_in,
	int image_width, int image_height, int image_stride,
	const st_mobile_106_t *p_faces_array_in, const int faces_count,
	unsigned char *img_out, st_pixel_format fmt_out,
	st_mobile_106_t *p_faces_array_out
);

/// @brief 对图像做美颜处理，此接口针对不在OpenGL环境中执行函数的用户,内部会初始化环境，较慢，只适合单张图片
/// @param[in] handle 已初始化的美颜句柄
/// @param[in] img_in 输入图片的数据数组
/// @param[in] fmt_in 输入图片的类型,支持NV21,BGR,BGRA,NV12,RGBA格式。
/// @param[in] image_width 输入图片的宽度(以像素为单位)
/// @param[in] image_height 输入图片的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位)，即每行的字节数；目前仅支持字节对齐的padding，不支持roi
/// @param[in] p_faces_array_in 输入需要美颜的人脸106点数组；如果为NULL，不执行大眼瘦脸
/// @param[in] faces_count 需要美颜的人脸数目；如果为0，不执行大眼瘦脸
/// @param[out] img_out 输出图像数据数组
/// @param[in] fmt_out 输出图片的类型,支持NV21,BGR,BGRA,NV12,RGBA格式
/// @param[out] p_faces_array_out 输出美颜后的人脸106点数组，需要由用户分配内存，必须与输入的人脸106点数组大小相同；如果为NULL，不输出美颜后的人脸106点数组
/// @return 成功返回ST_OK, 错误则返回错误码，错误码定义在st_mobile_common.h 中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_beautify_process_picture(
	st_handle_t handle,
	const unsigned char *img_in, st_pixel_format fmt_in,
	int image_width, int image_height, int image_stride,
	const st_mobile_106_t *p_faces_array_in, const int faces_count,
	unsigned char *img_out, st_pixel_format fmt_out,
	st_mobile_106_t *p_faces_array_out
);

/// @brief 对NV12,NV21格式图像做美颜处理，此接口针对ios平台中Y数据块与UV数据块不连续存储的数据，例如分辨率1920*1080的NV12图像
/// @param[in] handle 已初始化的美颜句柄
/// @param[in] img_in_y 输入图片的Y数据指针
/// @param[in] img_in_uv 输入图片的UV数据指针
/// @param[in] fmt_in 输入图片的类型,支持NV21,NV12格式。
/// @param[in] image_width 输入图片的宽度(以像素为单位)
/// @param[in] image_height 输入图片的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位)，即每行的字节数；目前仅支持字节对齐的padding，不支持roi
/// @param[out] img_out 输出图像数据数组
/// @param[in] fmt_out 输出图片的类型,支持NV21,BGR,BGRA,NV12,RGBA格式
/// @return 成功返回ST_OK, 错误则返回错误码，错误码定义在st_mobile_common.h 中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_beautify_process_nvbuffer(
	st_handle_t handle,
	const unsigned char *img_in_y, unsigned char *img_in_uv, st_pixel_format fmt_in,
	int image_width, int image_height, int image_stride,
	unsigned char *img_out, st_pixel_format fmt_out
);

/// @brief 对OpenGL ES 中的纹理进行美颜处理
/// @param[in] handle 已初始化的美颜句柄
/// @param[in] textureid_src 待处理的纹理id, 仅支持RGBA纹理
/// @param[in] image_width 输入纹理的宽度(以像素为单位)
/// @param[in] image_height 输入纹理的高度(以像素为单位)
/// @param[in] p_faces_array_in 输入需要美颜的人脸106点数组；如果为NULL，不执行大眼瘦脸
/// @param[in] faces_count 需要美颜的人脸数目；如果为0，不执行大眼瘦脸
/// @param[in] textureid_dst 处理后的纹理id，仅支持RGBA处理
/// @param[out] p_faces_array_out 输出美颜后的人脸106点数组，需要由用户分配内存，必须与输入的人脸106点数组大小相同；如果为NULL，不输出美颜后的人脸106点数组
/// @return 成功返回ST_OK, 错误则返回错误码，错误码定义在st_common.h 中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_beautify_process_texture(
	st_handle_t handle,
	unsigned int textureid_src,
	int image_width, int image_height,
	const st_mobile_106_t *p_faces_array_in, const int faces_count,
	unsigned int textureid_dst,
	st_mobile_106_t *p_faces_array_out
);

/// @brief 对OpenGL ES 中的纹理进行美颜处理
/// @param[in] handle 已初始化的美颜句柄
/// @param[in] textureid_src 待处理的纹理id, 仅支持RGBA纹理
/// @param[in] image_width 输入纹理的宽度(以像素为单位)
/// @param[in] image_height 输入纹理的高度(以像素为单位)
/// @param[in] p_faces_array_in 输入需要美颜的人脸106点数组；如果为NULL，不执行大眼瘦脸
/// @param[in] faces_count 需要美颜的人脸数目；如果为0，不执行大眼瘦脸
/// @param[in] textureid_dst 处理后的纹理id，仅支持RGBA处理
/// @param[out] img_out 输出图像数据数组,需要用户分配内存,如果是null, 不输出buffer
/// @param[out] p_faces_array_out 输出美颜后的人脸106点数组，需要由用户分配内存，必须与输入的人脸106点数组大小相同；如果为NULL，不输出美颜后的人脸106点数组
/// @param[in] fmt_out 输出图片的类型,支持NV21,BGR,BGRA,NV12,RGBA格式。
/// @return 成功返回ST_OK, 错误则返回错误码，错误码定义在st_common.h 中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_beautify_process_and_output_texture(
	st_handle_t handle,
	unsigned int textureid_src,
	int image_width, int image_height,
	const st_mobile_106_t *p_faces_array_in, const int faces_count,
	unsigned int textureid_dst,
	unsigned char *img_out, st_pixel_format fmt_out,
	st_mobile_106_t *p_faces_array_out
);

/// @brief 释放美颜句柄
/// @param[in] handle 已初始化的美颜句柄
ST_SDK_API void
st_mobile_beautify_destroy(
	st_handle_t handle
);
/// @}

#endif	// INCLUDE_STMOBILE_ST_MOBILE_BEAUTIFY_H_
