//
// Created by yjz on 2019/6/29 0029.
//#include "fmod.hpp"
#include "inc/fmod.hpp" //TODO 怎么省去inc?
#include <jni.h>
#include <string>       //TODO <>和""差别
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
    void checkError(FMOD_RESULT result) {//TODO 声明在h文件里？
        LOGI("%d",result)
        if (result != FMOD_OK) {
            //TODO 抛出异常？
        }
    }

    void checkAndInit() {
        if (fmodSystem == NULL) {
            LOGI("%s","checkAndInit")
            //版本
            unsigned int version;
            //创建FMOD::System_Create对象引用
            FMOD_RESULT result = FMOD::System_Create(&fmodSystem);
            MyTools::checkError(result);

            LOGI("%s","getVersion")
            result = fmodSystem -> getVersion(&version);
            MyTools::checkError(result);
            if (version < FMOD_VERSION) {
                LOGE("%s","FMOD lib version  doesn't match header version ")
//        LOGE("%s","FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION)//TODO 这种日志怎么打印
                return;
            }

            LOGI("%s","init")
            //初始化
            result = fmodSystem->init(32, FMOD_INIT_NORMAL, NULL);
            MyTools::checkError(result);
        }
    }

    void release() {
        if (fmodSystem != NULL) {
            LOGI("%s", "release")
            //释放资源
            sound -> release();
            fmodSystem -> release();
            fmodSystem = NULL;
            sound = NULL;
            channel = NULL;
            dps = NULL;
        }
    }
}




extern "C"  //TODO 啥意思
JNIEXPORT void Java_com_yjz_ndk_fmodsample_FMODTools_play(JNIEnv *jniEnv, jclass type, jstring filePath_,
                                                                 jint mode) {

    LOGI("%s","changeVoice start")
    //速度
    float frequency;
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
            channel -> getFrequency(&frequency);
  //          LOGI("frequency=%d",frequency)
//            result = channel -> setFrequency(frequency * 2);
//
//            result =  channel -> getFrequency(&frequency);
//            LOGI("frequency2=%d",frequency)
//            checkError(result);
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
  //  MyTools::release();
    LOGI("%s","changeVoice emd")
}

extern "C"
JNIEXPORT void JNICALL
Java_com_yjz_ndk_fmodsample_FMODTools_release(JNIEnv *env, jclass type) {
    //释放资源
    MyTools::release();
}