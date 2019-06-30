package com.yjz.ndk.fmodsample;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private final String TAG = "yyMainActivity";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void playTest(View view) {
        Log.d(TAG, "click playTest");
        String path = Environment.getExternalStorageDirectory().getPath()+ File.separator+"wave.mp3";
        FMODTools.changeVoice(path, 2);
    }
}
