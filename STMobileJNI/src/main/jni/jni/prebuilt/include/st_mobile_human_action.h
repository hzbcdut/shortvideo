
#ifndef INCLUDE_STMOBILE_API_ST_MOBILE_HUMAN_ACTION_H_
#define INCLUDE_STMOBILE_API_ST_MOBILE_HUMAN_ACTION_H_

#include "st_common.h"

/// @defgroup st_mobile_human_action
/// @brief human action detection interfaces
///
/// This set of interfaces detect human action.
///
/// @{
#define CHECK_FLAG(action,flag) (((action)&(flag)) == flag)
///@brief 人体行为类型
// face 0x00000000-0x000000FF
#define ST_MOBILE_FACE_DETECT      0x00000001    ///<  人脸检测
#define ST_MOBILE_EYE_BLINK        0x00000002    ///<  眨眼
#define ST_MOBILE_MOUTH_AH         0x00000004    ///<  嘴巴大张
#define ST_MOBILE_HEAD_YAW         0x00000008    ///<  摇头
#define ST_MOBILE_HEAD_PITCH       0x00000010    ///<  点头
#define ST_MOBILE_BROW_JUMP        0x00000020    ///<  眉毛挑动
#define ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG   0x000000FF    ///<  全部检测


#define ST_MOBILE_HUMAN_ACTION_MAX_FACE_COUNT 10

/// face信息及face上的相关动作
typedef struct st_mobile_face_action_t {
	struct st_mobile_106_t face;          /// 人脸信息，包含矩形、106点、pose信息等//
	unsigned int face_action;             /// 脸部动作
} st_mobile_face_action_t;

/// 视频分析结果
typedef struct st_mobile_human_action_t {
	st_mobile_face_action_t faces[ST_MOBILE_HUMAN_ACTION_MAX_FACE_COUNT];            /// 检测到的人脸及动作数组
	int face_count;                                                                  /// 检测到的人脸数目
} st_mobile_human_action_t, *p_st_mobile_human_action_t;

/// @brief 创建人体行为检测句柄
/// @param[in] model_path 模型文件的例如models/track.tar
/// @param[in] config 配置选项，分别代表每种状态是否被检测，例如ST_MOBILE_DEFAULT_CONFIG 或 ST_MOBILE_EYE_BLINK|ST_MOBILE_MOUTH_AH
/// @parma[out] handle 人体行为检测句柄，失败返回NULL
/// @return 成功返回ST_OK, 失败返回其他错误码,错误码定义在st_common.h 中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_human_action_create(
	const char *model_path,
	unsigned int config,
	st_handle_t *handle
);

/// @brief 释放人体行为检测句柄
/// @param[in] handle 已初始化的人体行为句柄
ST_SDK_API
void st_mobile_human_action_destroy(
	st_handle_t handle
);

/// @brief 人体行为检测
/// @param[in] handle 已初始化的人体行为句柄
/// @param[in] image 用于检测的图像数据
/// @param[in] pixel_format 用于检测的图像数据的像素格式,都支持
/// @param[in] image_width 用于检测的图像的宽度(以像素为单位)
/// @param[in] image_height 用于检测的图像的高度(以像素为单位)
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位)，即每行的字节数；目前仅支持字节对齐的padding，不支持roi
/// @param[in] orientation 图像中人脸的方向
/// @param[in] detect_config 检测选项，代表当前需要检测哪些动作，例如ST_MOBILE_EYE_BLINK|ST_MOBILE_MOUTH_AH表示当前帧只检测眨眼和张嘴
/// @param[out] p_humans_array 检测到的人体行为数组,用户负责分配内存
/// @param[out] p_humans_count 检测到的人数量
/// @return 成功返回ST_OK，失败返回其他错误码,错误码定义在st_common.h 中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_human_action_detect(
	st_handle_t handle,
	const unsigned char *image,
	st_pixel_format pixel_format,
	int image_width,
	int image_height,
	int image_stride,
	st_rotate_type orientation,
	unsigned int detect_config,
	st_mobile_human_action_t *p_human_action
);

///@brief 重置，清除所有缓存信息
ST_SDK_API st_result_t
st_mobile_human_action_reset(
	st_handle_t handle
);

/// @}s

//>> CONFIG_API_END__

#endif  // INCLUDE_STMOBILE_API_ST_MOBILE_H_
