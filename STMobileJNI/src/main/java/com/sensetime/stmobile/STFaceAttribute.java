package com.sensetime.stmobile;


/**
 * 定义人脸属性信息，比如年龄，性别，颜值
 */

public class STFaceAttribute {
    public int attribute_count;
    public Attribute[] arrayAttribute;

    public static class Attribute {
        public String category; // 属性描述, 如"age", "gender", "attractive"
        public String label; //属性标签描述， 如"male", "female"，"35"等
        public float score; //该属性标签的置信度
    }
}
