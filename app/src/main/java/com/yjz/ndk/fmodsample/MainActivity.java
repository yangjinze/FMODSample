package com.yjz.ndk.fmodsample;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.RadioGroup;
import android.widget.SeekBar;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private final String TAG = "yyMainActivity";
    private SeekBar seekBarPitch;
    private TextView tvPitch;
    private RadioGroup rgModel;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvPitch = findViewById(R.id.tv_pitch);
        seekBarPitch = findViewById(R.id.seekBar_pitch);
        rgModel = findViewById(R.id.rg_model);


        seekBarPitch.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                Log.d(TAG, "onProgressChanged " + progress);
                tvPitch.setText(progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                Log.d(TAG, "onStartTrackingTouch ");
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Log.d(TAG, "onStopTrackingTouch ");
            }
        });
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
                String path = Environment.getExternalStorageDirectory().getPath()+ File.separator+"wave.mp3";
                FMODTools.play(path, getPlayModel());
            }
        }).start();
    }

    public void stop(View view) {
        FMODTools.release();
    }
}
