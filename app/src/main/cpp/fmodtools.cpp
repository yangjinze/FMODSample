//
// Created by yjz on 2019/6/29 0029.
//#include "fmod.hpp"
#include "inc/fmod.hpp"
#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include <locale>

#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"yyFMODTools.cpp",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"yyFMODTools.cpp",FORMAT,##__VA_ARGS__);

// 定义五种不同的模式
//正常
#define MODE_NORMAL 0
//萝莉
#define MODE_LUOLI 1
//大叔
#define MODE_DASHU 2
//惊悚
#define MODE_JINGSONG 3
//搞怪
#define MODE_GAOGUAI 4
//空灵
#define MODE_KONGLING 5

using namespace FMOD;
FMOD::System *fmodSystem;
FMOD::Sound *sound;
// 通道（声音是由多种音效组成）
FMOD::Channel *channel;
//音频
FMOD::DSP *dps;

namespace MyTools {

    bool isPlaying = false;

    void checkAndInit() {
        if (!isPlaying) {
            LOGI("%s","checkAndInit")
            //版本
            unsigned int version;
            //创建FMOD::System_Create对象引用
            FMOD::System_Create(&fmodSystem);

            LOGI("%s","getVersion")
            fmodSystem -> getVersion(&version);
            if (version < FMOD_VERSION) {
                LOGE("%s","FMOD lib version  doesn't match header version ")
                return;
            }

            LOGI("%s","init")
            //初始化
            fmodSystem->init(32, FMOD_INIT_NORMAL, NULL);
            isPlaying = true;
        }
    }

    void release() {
        if (isPlaying) {
            LOGI("%s", "release")
            //释放资源
            sound -> release();
            fmodSystem -> release();
            fmodSystem = NULL;
            sound = NULL;
            channel = NULL;
            dps = NULL;
            isPlaying = false;
        }
    }
}

extern "C"
JNIEXPORT void Java_com_yjz_ndk_fmodsample_FMODTools_play(JNIEnv *jniEnv, jclass type, jstring filePath_,
                                                                 jint mode, jobject playListener) {
    LOGI("%s","play start")
    MyTools::checkAndInit();
    LOGI("%s","createSound")
    // 将 jstring转为 char
    const char *path = jniEnv->GetStringUTFChars(filePath_, NULL);
    //创建Sound引用
    fmodSystem->createSound(path, FMOD_DEFAULT, NULL, &sound);

    switch (mode) {
        case MODE_NORMAL:
            LOGI("%s","MODE_NORMAL")
            fmodSystem ->playSound(sound, NULL, false, &channel);
            break;
        case MODE_LUOLI:
            LOGI("%s","MODE_LUOLI")
            // 设置音调，调高音调
            fmodSystem->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dps);
            dps -> setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 3.0);
            fmodSystem ->playSound(sound, NULL, false, &channel);
            channel->addDSP(0, dps);
            break;
        case MODE_DASHU:
            LOGI("%s","MODE_DASHU");
            // 设置音调，调低音调
            fmodSystem->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dps);
            dps -> setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7);
            fmodSystem ->playSound(sound, NULL, false, &channel);
            channel->addDSP(0, dps);
            break;
        case MODE_JINGSONG:
            LOGI("%s","MODE_JINGSONG");
            //速度
            float frequency;
            // 设置颤抖
            fmodSystem->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dps);
            // 设置颤抖的频率
            dps->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8);
            fmodSystem->playSound(sound, NULL, false, &channel);
            channel->addDSP(0, dps);
            channel->getFrequency(&frequency);
            frequency = frequency * 0.5;
            channel->setFrequency(frequency);
            break;
        case MODE_GAOGUAI:
            LOGI("%s","MODE_GAOGUAI");
            fmodSystem->createDSPByType(FMOD_DSP_TYPE_NORMALIZE, &dps);
            fmodSystem->playSound(sound, NULL, false, &channel);
            channel->addDSP(0, dps);
            // 获取速度并加速
            channel->getFrequency(&frequency);
            frequency = frequency * 1.6;
            channel->setFrequency(frequency);
            break;
        case MODE_KONGLING:
            LOGI("%s","MODE_KONGLING");
            // 设置重复
            fmodSystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &dps);
            // 设置重复的重复延迟
            dps->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
            dps->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20);
            fmodSystem->playSound(sound, NULL, false, &channel);
            channel->addDSP(0, dps);
            break;
    }


    fmodSystem->update();
    bool isPlaying = true;
    // 需要等待，等声音全部播放完成
    // 检查是否播放完成
    while (isPlaying) {
        channel->isPlaying(&isPlaying);
        usleep(1000);
    }
    MyTools::isPlaying = false;
  //  MyTools::release();
    LOGI("%s","play end")
    jclass  playListenerClass = jniEnv -> GetObjectClass(playListener);
    if (playListenerClass == NULL) {
        LOGE("%s","can't find playListener class !!")
        return;
    }
    jmethodID complete = jniEnv -> GetMethodID(playListenerClass, "complete", "()V");
    if (complete == NULL) {
        LOGE("%s","can't find complete method !!")
        return;
    }
    jniEnv -> CallVoidMethod(playListener, complete);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yjz_ndk_fmodsample_FMODTools_release(JNIEnv *env, jclass type) {
    //释放资源
    MyTools::release();
}