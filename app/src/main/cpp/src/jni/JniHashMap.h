//
// Created by wangyuelin on 2022/9/17.
//

#ifndef IAUDIOPALYER_JNIHASHMAP_H
#define IAUDIOPALYER_JNIHASHMAP_H

#include <jni.h>

class JniHashMap {
protected:
    JNIEnv *env;
    jclass class_hashmap;
    jobject object_hashmap;

protected:
    /**
     * 检查指针是否为空
     * @param ptr
     * @param msg
     */
    void null_check(void *ptr, const char *msg);
    void null_check(void *ptr);

public:
    JniHashMap(JNIEnv *env);

    virtual ~JniHashMap();

    /**
     * 添加一个键值对
     * @param key
     * @param value
     */
    bool put(const char *key, const char *value);

    /**
     * 获取java的HashMap对象
     * @return
     */
    jobject getHashMap();
};


#endif //IAUDIOPALYER_JNIHASHMAP_H
