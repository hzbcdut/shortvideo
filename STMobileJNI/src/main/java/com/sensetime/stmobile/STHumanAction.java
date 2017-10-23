package com.sensetime.stmobile;

import com.sensetime.stmobile.model.STMobile106;
import com.sensetime.stmobile.model.STMobileFaceAction;

/**
 * 人脸信息定义（包括人脸信息及人脸行为），
 * 作为STMobileHumanActionNative.humanActionDetect的返回值
 */
public class STHumanAction {
    public static final int POS_LEFT_EYE = 104;
    public static final int POS_RIGHT_EYE = 105;
    public static final int POS_MOUTH = 98;
    public STMobileFaceAction[] faces;
    public int faceCount;

    public STMobile106[] getMobileFaces() {
        if (faceCount == 0) {
            return null;
        }

        STMobile106[] arrayFaces = new STMobile106[faceCount];
        for(int i = 0; i < faceCount; ++i) {
            arrayFaces[i] = faces[i].face;
        }

        return arrayFaces;
    }

    public boolean replaceMobile106(STMobile106[] arrayFaces) {
        if (arrayFaces == null || arrayFaces.length == 0
                || faces == null || faceCount != arrayFaces.length) {
            return false;
        }

        for (int i = 0; i < arrayFaces.length; ++i) {
            faces[i].face = arrayFaces[i];
        }
        return true;
    }
}
