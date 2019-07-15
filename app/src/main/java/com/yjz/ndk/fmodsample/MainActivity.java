package com.yjz.ndk.fmodsample;

import android.content.res.AssetManager;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.RadioGroup;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
//参考https://github.com/huannan/VoiceChange
public class MainActivity extends AppCompatActivity {

    private final String TAG = "yyMainActivity";
    private RadioGroup rgModel;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        rgModel = findViewById(R.id.rg_model);
    }

    private int getPlayModel() {
        for (int index = 0 ; index < rgModel.getChildCount(); index++) {
            View child = rgModel.getChildAt(index);
            if (child.getId() == rgModel.getCheckedRadioButtonId()) {
                return index;
            }
        }
        return 0;
    }

    public void play(View view) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                saveMp3ToSdcard();
                String path = Environment.getExternalStorageDirectory().getPath()+ File.separator+"helloWorld.mp3";
                FMODTools.play(path, getPlayModel(), new FMODTools.PlayListener() {
                    @Override
                    public void complete() {
                        Log.d(TAG, "播放结束 ");
                        FMODTools.release();
                    }
                });
            }
        }).start();
    }

    public void stop(View view) {
        FMODTools.release();
    }


    private void saveMp3ToSdcard() {
        try {
            File mp3 = new File(Environment.getExternalStorageDirectory().getPath()+ File.separator+"helloWorld.mp3");
            if (!mp3.exists()) {
                Log.d(TAG,"释放mp3文件");
                AssetManager assetManager = getAssets();
                InputStream inputStream = assetManager.open("helloWorld.mp3");
                FileOutputStream fileOutputStream = new FileOutputStream(mp3);
                byte[] buffer = new byte[1024];
                int byteCount;
                while ((byteCount = inputStream.read(buffer)) != -1) {
                    fileOutputStream.write(buffer, 0, byteCount);
                }
                fileOutputStream.flush();
                inputStream.close();
                fileOutputStream.close();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
