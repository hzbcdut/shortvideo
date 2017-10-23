#include "utils.h"
#include "st_mobile_common.h"

#define  LOG_TAG    "utils"

long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int getImageStride(const st_pixel_format &pixel_format, const int &outputWidth) {
    int stride = 0;
    switch(pixel_format)
    {
        case ST_PIX_FMT_YUV420P:
        case ST_PIX_FMT_NV12:
        case ST_PIX_FMT_NV21:
            stride = outputWidth;
            break;
        case ST_PIX_FMT_BGRA8888:
        case ST_PIX_FMT_RGBA8888:
            stride = outputWidth * 4;
            break;
        case ST_PIX_FMT_BGR888:
            stride = outputWidth * 3;
            break;
        default:
            break;
    }

    return stride;
}

jobject convert2HumanAction(JNIEnv *env, const st_mobile_human_action_t &human_action)
{
    jclass humanActionCls = env->FindClass("com/sensetime/stmobile/STHumanAction");
    if (humanActionCls == NULL) {
        LOGE("STHumanAction class not found!");
        return NULL;
    }

    jobject humanActionObject = env->AllocObject(humanActionCls);

    jfieldID fieldFaceCount = env->GetFieldID(humanActionCls, "faceCount", "I");
    jfieldID fieldHumanAction = env->GetFieldID(humanActionCls, "faces", "[Lcom/sensetime/stmobile/model/STMobileFaceAction;");

    // human action face info
    jclass humanActionFaceCls = env->FindClass("com/sensetime/stmobile/model/STMobileFaceAction");
    if (humanActionFaceCls == NULL) {
        LOGE("HumanActionFace class not found!");
        return NULL;
    }
    int face_count = human_action.face_count;
    jobjectArray humanActionFaces_array = env->NewObjectArray(face_count, humanActionFaceCls, 0);

    jfieldID fieldFace106 = env->GetFieldID(humanActionFaceCls, "face", "Lcom/sensetime/stmobile/model/STMobile106;");
    jfieldID fieldFaceAction = env->GetFieldID(humanActionFaceCls, "face_action", "I");

    // 106 point
    jclass st_mobile_106_class = env->FindClass("com/sensetime/stmobile/model/STMobile106");
    if (st_mobile_106_class == NULL) {
        LOGE("STFace106 class not found!");
        return NULL;
    }
    jfieldID frect = env->GetFieldID(st_mobile_106_class, "rect", "Lcom/sensetime/stmobile/model/STRect;");
    jfieldID fscore = env->GetFieldID(st_mobile_106_class, "score", "F");
    jfieldID fpoints_array = env->GetFieldID(st_mobile_106_class, "points_array", "[Lcom/sensetime/stmobile/model/STPoint;");
    jfieldID fyaw = env->GetFieldID(st_mobile_106_class, "yaw", "F");
    jfieldID fpitch = env->GetFieldID(st_mobile_106_class, "pitch", "F");
    jfieldID froll = env->GetFieldID(st_mobile_106_class, "roll", "F");
    jfieldID feye_dist = env->GetFieldID(st_mobile_106_class, "eye_dist", "F");
    jfieldID fID = env->GetFieldID(st_mobile_106_class, "ID", "I");

    jclass st_mobile_point_class = env->FindClass("com/sensetime/stmobile/model/STPoint");
    jfieldID fpoint_x = env->GetFieldID(st_mobile_point_class, "x", "F");
    jfieldID fpoint_y = env->GetFieldID(st_mobile_point_class, "y", "F");

    jclass st_face_rect_class = env->FindClass("com/sensetime/stmobile/model/STRect");
    jfieldID frect_left = env->GetFieldID(st_face_rect_class, "left", "I");
    jfieldID frect_top = env->GetFieldID(st_face_rect_class, "top", "I");
    jfieldID frect_right = env->GetFieldID(st_face_rect_class, "right", "I");
    jfieldID frect_bottom = env->GetFieldID(st_face_rect_class, "bottom", "I");


    for (int i = 0; i < face_count; ++i)
    {
        jobject humanFace = env->AllocObject(humanActionFaceCls);

        struct st_mobile_106_t face = human_action.faces[i].face;
        jobject st_106_object = env->AllocObject(st_mobile_106_class);

        //继续获取rect
        jobject face_rect = env->AllocObject(st_face_rect_class);

        env->SetIntField(face_rect, frect_left, face.rect.left);
        env->SetIntField(face_rect, frect_right, face.rect.right);
        env->SetIntField(face_rect, frect_top, face.rect.top);
        env->SetIntField(face_rect, frect_bottom, face.rect.bottom);

        //继续获取points_array
        jobjectArray face_point_array = env->NewObjectArray(106, st_mobile_point_class, 0);
        for(int j=0; j<106; j++) {
            jobject st_point_object = env->AllocObject(st_mobile_point_class);

            env->SetFloatField(st_point_object, fpoint_x, face.points_array[j].x);
            env->SetFloatField(st_point_object, fpoint_y, face.points_array[j].y);

            env->SetObjectArrayElement(face_point_array, j, st_point_object);
            env->DeleteLocalRef(st_point_object);
        }
        env->SetObjectField(st_106_object, frect, face_rect);
        env->SetFloatField(st_106_object, fscore, face.score);
        env->SetObjectField(st_106_object, fpoints_array, face_point_array);
        env->SetFloatField(st_106_object, fyaw, face.yaw);
        env->SetFloatField(st_106_object, fpitch, face.pitch);
        env->SetFloatField(st_106_object, froll, face.roll);
        env->SetFloatField(st_106_object, feye_dist, face.eye_dist);
        env->SetIntField(st_106_object, fID, face.ID);


        env->SetIntField(humanFace, fieldFaceAction, human_action.faces[i].face_action);
        env->SetObjectField(humanFace, fieldFace106, st_106_object);
        env->SetObjectArrayElement(humanActionFaces_array, i, humanFace);

        env->DeleteLocalRef(st_106_object);
        env->DeleteLocalRef(face_rect);
        env->DeleteLocalRef(face_point_array);
        env->DeleteLocalRef(humanFace);
    }


    env->SetIntField(humanActionObject, fieldFaceCount, face_count);
    env->SetObjectField(humanActionObject, fieldHumanAction, humanActionFaces_array);

    env->DeleteLocalRef(humanActionFaces_array);

    if (st_mobile_point_class != NULL) {
        env->DeleteLocalRef(st_mobile_point_class);
    }

    if (st_face_rect_class != NULL) {
        env->DeleteLocalRef(st_face_rect_class);
    }

    if (st_mobile_106_class != NULL) {
        env->DeleteLocalRef(st_mobile_106_class);
    }

    if (humanActionFaceCls != NULL) {
        env->DeleteLocalRef(humanActionFaceCls);
    }

    if (humanActionCls != NULL) {
        env->DeleteLocalRef(humanActionCls);
    }
    return humanActionObject;
}
jobject convert2MobileFace106(JNIEnv *env, const st_mobile_106_t &mobile_106)
{
    jclass st_mobile_106_class = env->FindClass("com/sensetime/stmobile/model/STMobile106");
    jfieldID frect = env->GetFieldID(st_mobile_106_class, "rect", "Lcom/sensetime/stmobile/model/STRect;");
    jfieldID fscore = env->GetFieldID(st_mobile_106_class, "score", "F");
    jfieldID fpoints_array = env->GetFieldID(st_mobile_106_class, "points_array", "[Lcom/sensetime/stmobile/model/STPoint;");
    jfieldID fyaw = env->GetFieldID(st_mobile_106_class, "yaw", "F");
    jfieldID fpitch = env->GetFieldID(st_mobile_106_class, "pitch", "F");
    jfieldID froll = env->GetFieldID(st_mobile_106_class, "roll", "F");
    jfieldID feye_dist = env->GetFieldID(st_mobile_106_class, "eye_dist", "F");
    jfieldID fID = env->GetFieldID(st_mobile_106_class, "ID", "I");

    jclass st_mobile_point_class = env->FindClass("com/sensetime/stmobile/model/STPoint");
    jfieldID fpoint_x = env->GetFieldID(st_mobile_point_class, "x", "F");
    jfieldID fpoint_y = env->GetFieldID(st_mobile_point_class, "y", "F");

    jclass st_face_rect_class = env->FindClass("com/sensetime/stmobile/model/STRect");
    jfieldID frect_left = env->GetFieldID(st_face_rect_class, "left", "I");
    jfieldID frect_top = env->GetFieldID(st_face_rect_class, "top", "I");
    jfieldID frect_right = env->GetFieldID(st_face_rect_class, "right", "I");
    jfieldID frect_bottom = env->GetFieldID(st_face_rect_class, "bottom", "I");

    jobject st_106_object = env->AllocObject(st_mobile_106_class);

    //继续获取rect
    jobject face_rect = env->AllocObject(st_face_rect_class);

    env->SetIntField(face_rect, frect_left, mobile_106.rect.left);
    env->SetIntField(face_rect, frect_right, mobile_106.rect.right);
    env->SetIntField(face_rect, frect_top, mobile_106.rect.top);
    env->SetIntField(face_rect, frect_bottom, mobile_106.rect.bottom);

    //继续获取points_array
    jobjectArray face_point_array = env->NewObjectArray(106, st_mobile_point_class, 0);
    for(int j=0; j<106; j++) {
        jobject st_point_object = env->AllocObject(st_mobile_point_class);

        env->SetFloatField(st_point_object, fpoint_x, mobile_106.points_array[j].x);
        env->SetFloatField(st_point_object, fpoint_y, mobile_106.points_array[j].y);

        env->SetObjectArrayElement(face_point_array, j, st_point_object);
        env->DeleteLocalRef(st_point_object);
    }
    env->SetObjectField(st_106_object, frect, face_rect);
    env->SetFloatField(st_106_object, fscore, mobile_106.score);
    env->SetObjectField(st_106_object, fpoints_array, face_point_array);
    env->SetFloatField(st_106_object, fyaw, mobile_106.yaw);
    env->SetFloatField(st_106_object, fpitch, mobile_106.pitch);
    env->SetFloatField(st_106_object, froll, mobile_106.roll);
    env->SetFloatField(st_106_object, feye_dist, mobile_106.eye_dist);
    env->SetIntField(st_106_object, fID, mobile_106.ID);

    env->DeleteLocalRef(face_rect);
    env->DeleteLocalRef(face_point_array);

    if (st_mobile_point_class != NULL) {
        env->DeleteLocalRef(st_mobile_point_class);
    }

    if (st_face_rect_class != NULL) {
        env->DeleteLocalRef(st_face_rect_class);
    }

    if (st_mobile_106_class != NULL) {
        env->DeleteLocalRef(st_mobile_106_class);
    }

    return st_106_object;
}

jobject convert2FaceAttribute(JNIEnv *env, const st_mobile_attribute_t &faceAttribute)
{
    jclass face_attribute_cls = env->FindClass("com/sensetime/stmobile/STFaceAttribute");

    jfieldID fieldAttribute_count = env->GetFieldID(face_attribute_cls, "attribute_count", "I");
    jfieldID fieldAttribute = env->GetFieldID(face_attribute_cls, "arrayAttribute", "[Lcom/sensetime/stmobile/STFaceAttribute$Attribute;");

    jobject faceAttributeObj = env->AllocObject(face_attribute_cls);

    env->SetIntField(faceAttributeObj, fieldAttribute_count, faceAttribute.attribute_count);

    jclass attribute_cls = env->FindClass("com/sensetime/stmobile/STFaceAttribute$Attribute");
    jfieldID fieldCategory = env->GetFieldID(attribute_cls, "category", "Ljava/lang/String;");
    jfieldID fieldLabel = env->GetFieldID(attribute_cls, "label", "Ljava/lang/String;");
    jfieldID fieldScore = env->GetFieldID(attribute_cls, "score", "F");

    if (faceAttribute.attribute_count > 0) {
        jobjectArray arrayAttrObj = env->NewObjectArray(faceAttribute.attribute_count, attribute_cls, 0);
        for (int i = 0; i < faceAttribute.attribute_count; ++i) {
            st_mobile_attribute_one one = faceAttribute.attributes[i];
            jobject attrObj = env->AllocObject(attribute_cls);
            jstring cateObj = env->NewStringUTF(one.category);
            jstring labelObj = env->NewStringUTF(one.label);
            env->SetObjectField(attrObj, fieldCategory, cateObj);
            env->SetObjectField(attrObj, fieldLabel, labelObj);
            env->SetFloatField(attrObj, fieldScore, one.score);

            env->SetObjectArrayElement(arrayAttrObj, i, attrObj);
            env->DeleteLocalRef(cateObj);
            env->DeleteLocalRef(labelObj);
            env->DeleteLocalRef(attrObj);
        }

        env->SetObjectField(faceAttributeObj, fieldAttribute, arrayAttrObj);
        env->DeleteLocalRef(arrayAttrObj);
    }

    env->DeleteLocalRef(attribute_cls);
    env->DeleteLocalRef(face_attribute_cls);
    return faceAttributeObj;
}

bool convert2human_action(JNIEnv *env, jobject humanActionObject, st_mobile_human_action_t &human_action)
{
    if (humanActionObject == NULL) {
        return false;
    }
    jclass humanActionCls = env->GetObjectClass(humanActionObject);

    jfieldID fieldFaceCount = env->GetFieldID(humanActionCls, "faceCount", "I");
    jfieldID fieldHumanAction = env->GetFieldID(humanActionCls, "faces", "[Lcom/sensetime/stmobile/model/STMobileFaceAction;");

    human_action.face_count = env->GetIntField(humanActionObject, fieldFaceCount);
    jobjectArray humanActionArray = (jobjectArray)env->GetObjectField(humanActionObject, fieldHumanAction);

    // human action face info
    jclass humanActionFaceCls = env->FindClass("com/sensetime/stmobile/model/STMobileFaceAction");
    if (humanActionFaceCls == NULL) {
        LOGE("HumanActionFace class not found!");
        return false;
    }

    jfieldID fieldFace106 = env->GetFieldID(humanActionFaceCls, "face", "Lcom/sensetime/stmobile/model/STMobile106;");
    jfieldID fieldFaceAction = env->GetFieldID(humanActionFaceCls, "face_action", "I");

    // 106 point
    jclass st_mobile_106_class = env->FindClass("com/sensetime/stmobile/model/STMobile106");
    jfieldID frect = env->GetFieldID(st_mobile_106_class, "rect", "Lcom/sensetime/stmobile/model/STRect;");
    jfieldID fscore = env->GetFieldID(st_mobile_106_class, "score", "F");
    jfieldID fpoints_array = env->GetFieldID(st_mobile_106_class, "points_array", "[Lcom/sensetime/stmobile/model/STPoint;");
    jfieldID fyaw = env->GetFieldID(st_mobile_106_class, "yaw", "F");
    jfieldID fpitch = env->GetFieldID(st_mobile_106_class, "pitch", "F");
    jfieldID froll = env->GetFieldID(st_mobile_106_class, "roll", "F");
    jfieldID feye_dist = env->GetFieldID(st_mobile_106_class, "eye_dist", "F");
    jfieldID fID = env->GetFieldID(st_mobile_106_class, "ID", "I");

    jclass st_mobile_point_class = env->FindClass("com/sensetime/stmobile/model/STPoint");
    jfieldID fpoint_x = env->GetFieldID(st_mobile_point_class, "x", "F");
    jfieldID fpoint_y = env->GetFieldID(st_mobile_point_class, "y", "F");

    jclass st_face_rect_class = env->FindClass("com/sensetime/stmobile/model/STRect");
    jfieldID frect_left = env->GetFieldID(st_face_rect_class, "left", "I");
    jfieldID frect_top = env->GetFieldID(st_face_rect_class, "top", "I");
    jfieldID frect_right = env->GetFieldID(st_face_rect_class, "right", "I");
    jfieldID frect_bottom = env->GetFieldID(st_face_rect_class, "bottom", "I");

    for (int i = 0; i < human_action.face_count; ++i)
    {
        jobject humanActionFace = env->GetObjectArrayElement(humanActionArray, i);

        human_action.faces[i].face_action = env->GetIntField(humanActionFace, fieldFaceAction);

        jobject face106 = env->GetObjectField(humanActionFace, fieldFace106);
        human_action.faces[i].face.score = env->GetFloatField(face106, fscore);
        human_action.faces[i].face.yaw = env->GetFloatField(face106, fyaw);
        human_action.faces[i].face.pitch = env->GetFloatField(face106, fpitch);
        human_action.faces[i].face.roll = env->GetFloatField(face106, froll);
        human_action.faces[i].face.eye_dist = env->GetFloatField(face106, feye_dist);
        human_action.faces[i].face.ID = env->GetIntField(face106, fID);

        jobject faceRect = env->GetObjectField(face106, frect);
        human_action.faces[i].face.rect.left = env->GetIntField(faceRect, frect_left);
        human_action.faces[i].face.rect.right = env->GetIntField(faceRect, frect_right);
        human_action.faces[i].face.rect.top = env->GetIntField(faceRect, frect_top);
        human_action.faces[i].face.rect.bottom = env->GetIntField(faceRect, frect_bottom);

        jobjectArray points_array = (jobjectArray)env->GetObjectField(face106, fpoints_array);
        for (int j = 0; j < 106; ++j)
        {
            jobject point = env->GetObjectArrayElement(points_array, j);

            human_action.faces[i].face.points_array[j].x = env->GetFloatField(point, fpoint_x);
            human_action.faces[i].face.points_array[j].y = env->GetFloatField(point, fpoint_y);
            env->DeleteLocalRef(point);
        }

        env->DeleteLocalRef(humanActionFace);
        env->DeleteLocalRef(face106);
        env->DeleteLocalRef(faceRect);
        env->DeleteLocalRef(points_array);

    }

    env->DeleteLocalRef(st_mobile_106_class);
    env->DeleteLocalRef(st_face_rect_class);
    env->DeleteLocalRef(st_mobile_point_class);
    env->DeleteLocalRef(humanActionArray);
    if (humanActionCls != NULL) {
        env->DeleteLocalRef(humanActionCls);
    }

    return true;
}

bool convert2mobile_106(JNIEnv *env, jobject face106, st_mobile_106_t &mobile_106)
{
    if (face106 == NULL) {
        return false;
    }

    jclass st_mobile_106_class = env->FindClass("com/sensetime/stmobile/model/STMobile106");
    jfieldID frect = env->GetFieldID(st_mobile_106_class, "rect", "Lcom/sensetime/stmobile/model/STRect;");
    jfieldID fscore = env->GetFieldID(st_mobile_106_class, "score", "F");
    jfieldID fpoints_array = env->GetFieldID(st_mobile_106_class, "points_array", "[Lcom/sensetime/stmobile/model/STPoint;");
    jfieldID fyaw = env->GetFieldID(st_mobile_106_class, "yaw", "F");
    jfieldID fpitch = env->GetFieldID(st_mobile_106_class, "pitch", "F");
    jfieldID froll = env->GetFieldID(st_mobile_106_class, "roll", "F");
    jfieldID feye_dist = env->GetFieldID(st_mobile_106_class, "eye_dist", "F");
    jfieldID fID = env->GetFieldID(st_mobile_106_class, "ID", "I");

    jclass st_mobile_point_class = env->FindClass("com/sensetime/stmobile/model/STPoint");
    jfieldID fpoint_x = env->GetFieldID(st_mobile_point_class, "x", "F");
    jfieldID fpoint_y = env->GetFieldID(st_mobile_point_class, "y", "F");

    jclass st_face_rect_class = env->FindClass("com/sensetime/stmobile/model/STRect");
    jfieldID frect_left = env->GetFieldID(st_face_rect_class, "left", "I");
    jfieldID frect_top = env->GetFieldID(st_face_rect_class, "top", "I");
    jfieldID frect_right = env->GetFieldID(st_face_rect_class, "right", "I");
    jfieldID frect_bottom = env->GetFieldID(st_face_rect_class, "bottom", "I");

    mobile_106.score = env->GetFloatField(face106, fscore);
    mobile_106.yaw = env->GetFloatField(face106, fyaw);
    mobile_106.pitch = env->GetFloatField(face106, fpitch);
    mobile_106.roll = env->GetFloatField(face106, froll);
    mobile_106.eye_dist = env->GetFloatField(face106, feye_dist);
    mobile_106.ID = env->GetIntField(face106, fID);

    jobject faceRect = env->GetObjectField(face106, frect);
    mobile_106.rect.left = env->GetIntField(faceRect, frect_left);
    mobile_106.rect.right = env->GetIntField(faceRect, frect_right);
    mobile_106.rect.top = env->GetIntField(faceRect, frect_top);
    mobile_106.rect.bottom = env->GetIntField(faceRect, frect_bottom);

    jobjectArray points_array = (jobjectArray)env->GetObjectField(face106, fpoints_array);
    for (int j = 0; j < 106; ++j)
    {
        jobject point = env->GetObjectArrayElement(points_array, j);

        mobile_106.points_array[j].x = env->GetFloatField(point, fpoint_x);
        mobile_106.points_array[j].y = env->GetFloatField(point, fpoint_y);
        env->DeleteLocalRef(point);
    }

    env->DeleteLocalRef(faceRect);
    env->DeleteLocalRef(points_array);
    env->DeleteLocalRef(st_mobile_106_class);
    env->DeleteLocalRef(st_face_rect_class);
    env->DeleteLocalRef(st_mobile_point_class);

    return true;
}
