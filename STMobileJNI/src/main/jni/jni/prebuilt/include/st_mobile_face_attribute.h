#ifndef INCLUDE_STMOBILE_ST_MOBILE_FACE_ATTRIBUTE_H_
#define INCLUDE_STMOBILE_ST_MOBILE_FACE_ATTRIBUTE_H_

#include "st_mobile_common.h"

/// @defgroup st_mobile_face_attribute
/// @brief face attribute detection interfaces
///
/// This set of interfaces detect face attribute like age, gender, attractive.
///
/// @{

/// @brief 创建人脸属性检测句柄
/// @param[in] model_path 模型文件的绝对路径或相对路径
/// @param[out] handle 人脸属性检测句柄
/// @return 成功返回ST_OK, 错误则返回错误码，错误码定义在st_mobile_common.h中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_face_attribute_create(
	const char *model_path,
	st_handle_t *handle
);
/// @brief 销毁已初始化的人脸属性检测句柄
/// @param[in] handle 已初始化的人脸属性检测句柄
ST_SDK_API void
st_mobile_face_attribute_destroy(
	st_handle_t handle
);
#ifndef ST_MOBILE_MAX_ATTR_STRING_LEN
#define ST_MOBILE_MAX_ATTR_STRING_LEN 32
#endif

// 属性
typedef struct st_mobile_attribute_one{
	char category[ST_MOBILE_MAX_ATTR_STRING_LEN];   ///< 属性描述, 如"age", "gender", "attractive"
	char label[ST_MOBILE_MAX_ATTR_STRING_LEN];      ///< 属性标签描述， 如"male", "female","21" 等
	float score;                           ///< 该属性标签的置信度
}st_mobile_attribute_one;

// 一个人脸的所有属性
typedef struct st_mobile_attribute_t {
	st_mobile_attribute_one* attributes;    ///< 属性数组
	int attribute_count;                    ///< 属性个数
}st_mobile_attribute_t;

/// @brief 人脸属性检测
/// @param[in] handle 已初始化的人脸属性检测句柄
/// @param[in] image 用于检测的图像数据,推荐灰度格式
/// @param[in] pixel_format 用于检测的图像数据的像素格式, 支持所有彩色图像格式，推荐ST_PIX_FMT_BGR888，不建议使用ST_PIX_FMT_GRAY8（结果不准确）
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像中每一行的跨度(以像素为单位)
/// @param[in] p_face_array 输入待处理的人脸信息，需要包括关键点信息
/// @param[in] face_count 输入的人脸个数
/// @param[out] p_attribute_array 检测到的人脸属性结果数组，包含face_count个st_mobile_attribute_t
/// @return 成功返回ST_OK, 错误则返回错误码，错误码定义在st_mobile_common.h中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_face_attribute_detect(
	st_handle_t handle,
	const unsigned char *image,
	st_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	const st_mobile_106_t *p_face_array,
	int face_count,
	st_mobile_attribute_t** p_attribute_array
);


/// @}


#endif  // INCLUDE_STMOBILE_ST_MOBILE_FACE_ATTRIBUTE_H_
