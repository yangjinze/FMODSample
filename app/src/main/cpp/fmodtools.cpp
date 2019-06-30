//
// Created by yjz on 2019/6/29 0029.
//#include "fmod.hpp"
#include "inc/fmod.hpp" //TODO 怎么省去inc?
#include <jni.h>
#include <string>       //TODO <>和""差别
#include <android/log.h>
#include <unistd.h>

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"yyFMODTools.cpp",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"yyFMODTools.cpp",FORMAT,##__VA_ARGS__);

void checkError(FMOD_RESULT result) {//TODO 声明在h文件里？
    LOGI("%d",result)
    if (result != FMOD_OK) {
        //TODO 抛出异常？
    }
}
using namespace FMOD;
extern "C"  //TODO 啥意思
JNIEXPORT void Java_com_yjz_ndk_fmodsample_FMODTools_changeVoice(JNIEnv *jniEnv, jclass type, jstring filePath_,
                                                                 jint mode) {

    LOGI("%s","changeVoice start")

    FMOD::System *system;
    FMOD::Sound *sound;
    // 通道（声音是由多种音效组成）
    Channel *channel;
    unsigned int version;

    LOGI("%s","System_Create")
    //创建FMOD::System_Create对象引用
    FMOD_RESULT result = FMOD::System_Create(&system);
    checkError(result);

    LOGI("%s","getVersion")
    result = system -> getVersion(&version);
    checkError(result);
    if (version < FMOD_VERSION) {
        LOGE("%s","FMOD lib version  doesn't match header version ")
//        LOGE("%s","FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION)//TODO 这种日志怎么打印
        return;
    }

    LOGI("%s","init")
    //初始化
    result = system->init(32, FMOD_INIT_NORMAL, NULL);
    checkError(result);

    LOGI("%s","createSound")
    // 将 jstring转为 char
    const char *path = jniEnv->GetStringUTFChars(filePath_, NULL);
    //创建Sound引用
    result = system->createSound(path, FMOD_DEFAULT, NULL, &sound);
    checkError(result);

    LOGI("%s","playSound")
    result = system ->playSound(sound, NULL, false, &channel);
    checkError(result);

    system->update();
    bool isPlaying = true;
    // 需要等待，等声音全部播放完成
    // 检查是否播放完成
    while (isPlaying) {
        channel->isPlaying(&isPlaying);
        usleep(1000);
    }
    //释放资源
    sound -> release();
    system -> release();
    LOGI("%s","changeVoice emd")
}

