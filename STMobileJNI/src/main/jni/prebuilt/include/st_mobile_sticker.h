#ifndef INCLUDE_STMOBILE_ST_MOBILE_STIKER_H_
#define INCLUDE_STMOBILE_ST_MOBILE_STIKER_H_

#include "st_mobile_common.h"
#include "st_mobile_face.h"

/// @defgroup st_mobile_human_action
/// @brief human action detection interfaces
///
/// This set of interfaces detect human action.
///
/// @{

/// @brief human_action配置选项

/// st_mobile_human_action_create的config参数默认配置, 请参考st_mobile_common.h中的track106配置选项
/// 使用多线程，默认检测face action，打开head_pose去抖动
/// 对视频进行检测推荐使用多线程
#define ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_VIDEO     ST_MOBILE_TRACKING_MULTI_THREAD | \
                                                        ST_MOBILE_TRACKING_ENABLE_FACE_ACTION | \
                                                        ST_MOBILE_TRACKING_ENABLE_DEBOUNCE
/// 对图片进行检测只能使用单线程
#define ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_IMAGE     ST_MOBILE_TRACKING_SINGLE_THREAD

#define ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_DETECT    0x000000FF  ///< 全部检测
#define ST_MOBILE_HUMAN_ACTION_MAX_FACE_COUNT           10

/// 检测结果
typedef struct st_mobile_human_action_t {
	st_mobile_face_action_t faces[ST_MOBILE_HUMAN_ACTION_MAX_FACE_COUNT];   /// 检测到的人脸及动作数组
	int face_count;                                                         /// 检测到的人脸数目
} st_mobile_human_action_t, *p_st_mobile_human_action_t;

/// @brief 创建人体行为检测句柄
/// @param[in] model_path 模型文件的路径，例如models/face_track_2.0.1.model
/// @param[in] config 设置创建人体行为句柄的方式，检测视频时设置为ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_CREATE，检测图片时设置为ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_IMAGE
/// @parma[out] handle 人体行为检测句柄，失败返回NULL
/// @return 成功返回ST_OK，失败返回其他错误码，错误码定义在st_mobile_common.h中，如ST_E_FAIL等
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
/// @param[in] detect_config 需要检测的人体行为，例如ST_MOBILE_HUMAN_ACTION_DEFAULT_CONFIG_DETECT或ST_MOBILE_EYE_BLINK | ST_MOBILE_MOUTH_AH
/// @param[out] p_human_action 检测到的人体行为，由用户分配内存
/// @return 成功返回ST_OK，失败返回其他错误码，错误码定义在st_mobile_common.h中，如ST_E_FAIL等
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

/// @defgroup st_mobile_sticker
/// @brief sticker for image interfaces
///
/// This set of interfaces sticker.
///

/// @brief 创建贴纸句柄
/// @param[in] zip_path 输入的素材包路径
/// @parma[out] handle 贴纸句柄，失败返回NULL
/// @return 成功返回ST_OK, 失败返回其他错误码,错误码定义在st_mobile_common.h中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_sticker_create(
	const char* zip_path,
	st_handle_t *handle
);

/// @brief 更换素材包,删除原有素材包
/// @parma[in] handle 已初始化的贴纸句柄
/// @param[in] zip_path 待更换的素材包文件夹
/// @return 成功返回ST_OK, 失败返回其他错误码,错误码定义在st_mobile_common.h中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_sticker_change_package(
	st_handle_t handle,
	const char* zip_path
);

/// @brief 获取触发动作类型
/// @parma[in] handle 已初始化的贴纸句柄
/// @param[out] action 返回的触发动作，每一位分别代表该位对应状态是否是触发动作，对应状态详见st_mobile_common.h中，如ST_MOBILE_EYE_BLINK等
/// @return 成功返回ST_OK, 失败返回其他错误码,错误码定义在st_mobile_common.h中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_sticker_get_trigger_action(
	st_handle_t handle,
	unsigned int *action
);

/// @brief 设置贴纸素材图像所占用的最大内存
/// @parma[in] handle 已初始化的贴纸句柄
/// @param[in] img_mem 贴纸素材图像所占用的最大内存（MB），默认150MB，素材过大时，循环加载，降低内存； 贴纸较小时，全部加载，降低cpu
/// @return 成功返回ST_OK, 失败返回其他错误码,错误码定义在st_mobile_common.h中，如ST_E_FAIL等
ST_SDK_API st_result_t
st_mobile_sticker_set_max_imgmem(
	st_handle_t handle,
	float img_mem
);

/// 素材渲染状态
typedef enum {
	ST_MATERIAL_BEGIN = 0,      ///< 开始渲染素材
	ST_MATERIAL_PROCESS = 1,    ///< 素材渲染中
	ST_MATERIAL_END = 2         ///< 素材未被渲染
} st_material_status;

/// 素材渲染状态回调函数
/// @param[in] material_name 素材文件夹名称
/// @param[in] status 素材渲染状态，详见st_material_status定义
typedef void(*item_action)(const char* material_name, st_material_status status);


/// @brief 对OpenGL ES 中的纹理进行贴纸处理，必须在opengl环境中运行，仅支持RGBA图像格式
/// @param[in]textureid_src 输入textureid
/// @param[in] image_width 图像宽度
/// @param[in] image_height 图像高度
/// @param[in] rotate 人脸朝向
/// @param[in] need_mirror 传入图像与显示图像是否是镜像关系
/// @param[in] human_action 动作，包含106点、face动作
/// @param[in] callback 素材渲染回调函数，由用户定义
/// @param[in]textureid_dst 输出textureid
ST_SDK_API st_result_t
st_mobile_sticker_process_texture(
	st_handle_t handle,
	unsigned int textureid_src, int image_width, int image_height,
	st_rotate_type rotate, bool need_mirror,
	p_st_mobile_human_action_t human_action,
	item_action callback,
	unsigned int textureid_dst
);

/// @brief 对OpenGL ES 中的纹理进行贴纸处理并转成buffer输出，必须在opengl环境中运行，仅支持RGBA图像格式的texture
/// @param[in] textureid_src 输入textureid
/// @param[in] image_width 图像宽度
/// @param[in] image_height 图像高度
/// @param[in] rotate 人脸朝向
/// @param[in] need_mirror 传入图像与显示图像是否是镜像关系
/// @param[in] human_action 动作，包含106点、face动作
/// @param[in] callback 素材渲染回调函数，由用户定义
/// @param[in] textureid_dst 输出textureid
/// @param[out] img_out 输出图像数据数组,需要用户分配内存,如果是null, 不输出buffer
/// @param[in] fmt_out 输出图片的类型,支持NV21,BGR,BGRA,NV12,RGBA格式。
ST_SDK_API st_result_t
st_mobile_sticker_process_and_output_texture(
	st_handle_t handle,
	unsigned int textureid_src, int image_width, int image_height,
	st_rotate_type rotate, bool need_mirror,
	p_st_mobile_human_action_t human_action,
	item_action callback,
	unsigned int textureid_dst,
	unsigned char* img_out, st_pixel_format fmt_out
);


/// @brief 释放贴纸句柄
ST_SDK_API void
st_mobile_sticker_destroy(
	st_handle_t handle
);



#endif  // INCLUDE_STMOBILE_ST_MOBILE_STIKER_H_
