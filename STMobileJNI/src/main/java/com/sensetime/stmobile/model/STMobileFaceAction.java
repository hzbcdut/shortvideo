package com.sensetime.stmobile.model;

/**
 * 定义人脸信息（包含106点及脸部动作）
 */
public class STMobileFaceAction {
    public STMobile106 face;           /// 人脸信息，包含矩形、106点、pose信息等
    public int face_action;             /// 脸部动作

    public STMobile106 getFace() {
        return face;
    }

    public void setFace(STMobile106 face) {
        this.face = face;
    }

    public int getFace_action() {
        return face_action;
    }

    public void setFace_action(int face_action) {
        this.face_action = face_action;
    }

}
