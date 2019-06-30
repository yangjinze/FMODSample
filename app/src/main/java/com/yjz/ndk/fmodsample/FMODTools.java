package com.yjz.ndk.fmodsample;

/**
 * FMOD工具集
 *
 * @author Yjz
 */
public class FMODTools {

    public static native void changeVoice(String filePath, int mode);

    static {
        System.loadLibrary("fmodtools");

        System.loadLibrary("fmod");
        System.loadLibrary("fmodL");
    }
}
