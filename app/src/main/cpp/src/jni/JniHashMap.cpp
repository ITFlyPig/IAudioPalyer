//
// Created by wangyuelin on 2022/9/17.
//


#include <stdexcept>
#include "JniHashMap.h"

JniHashMap::JniHashMap(JNIEnv *env) {
    this->env = env;
    null_check(env);
    jclass cls = env->FindClass("java/util/HashMap");
    class_hashmap = static_cast<jclass>(env->NewGlobalRef(cls));
    null_check(class_hashmap);
    jmethodID method_hashmap_constructor = env->GetMethodID(class_hashmap, "<init>", "()V");
    null_check(method_hashmap_constructor);
    jobject obj = env->NewObject(class_hashmap, method_hashmap_constructor);
    object_hashmap = env->NewGlobalRef(obj);
    null_check(object_hashmap);
}

void JniHashMap::null_check(void *ptr, const char *msg) {
    if (ptr == nullptr) {
        throw std::runtime_error(msg);
    }
}

void JniHashMap::null_check(void *ptr) {
    null_check(ptr, "指针不能为空");
}

bool JniHashMap::put(const char *key, const char *value) {
    if (key == nullptr || value == nullptr) {
        return false;
    }

    jmethodID method_put = env->GetMethodID(class_hashmap, "put",
                                            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    if (method_put == nullptr) return false;

    jstring jkey = env->NewStringUTF(key);
    jstring jvalue = env->NewStringUTF(value);
    env->CallObjectMethod(object_hashmap, method_put, jkey, jvalue);
    return true;
}

JniHashMap::~JniHashMap() {
    if (class_hashmap != nullptr) {
        env->DeleteGlobalRef(class_hashmap);
        class_hashmap = nullptr;
    }
    if (object_hashmap != nullptr) {
        env->DeleteLocalRef(object_hashmap);
        object_hashmap = nullptr;
    }
}

jobject JniHashMap::getHashMap() {
    return object_hashmap ;
}

