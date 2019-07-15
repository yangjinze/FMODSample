package com.yjz.ndk.fmodsample;

/**
 * FMOD工具集
 *
 * @author Yjz
 */
public class FMODTools {


    public static native void play(String filePath, int mode, PlayListener playListener);
    public static native void release();
    static {
        System.loadLibrary("fmodtools");
        System.loadLibrary("fmod");
        System.loadLibrary("fmodL");
    }

    public interface PlayListener {
        void complete();
    }
}
