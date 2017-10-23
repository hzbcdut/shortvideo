/*
 * Copyright (C) 2010-2017 Alibaba Group Holding Limited.
 */

package com.aliyun.demo.recorder;

import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;

import com.aliyun.common.utils.StorageUtils;
import com.aliyun.demo.R;
import com.aliyun.demo.recorder.util.Common;
import com.aliyun.struct.common.ScaleMode;
import com.aliyun.struct.common.VideoQuality;
import com.aliyun.struct.recorder.CameraType;
import com.aliyun.struct.recorder.FlashType;
import com.aliyun.struct.snap.AliyunSnapVideoParam;

import java.io.File;

/**
 * Created by Administrator on 2017/3/2.
 */

public class RecorderSettingTest extends Activity implements View.OnClickListener{
    String[] eff_dirs;

    private static final int PROGRESS_LOW = 25;
    private static final int PROGRESS_MEIDAN = 50;
    private static final int PROGRESS_HIGH = 75;
    private static final int PROGRESS_SUPER = 100;


    private static final int PROGRESS_360P = 25;
    private static final int PROGRESS_480P = 50;
    private static final int PROGRESS_540P = 75;
    private static final int PROGRESS_720P = 100;

    private static final int PROGRESS_3_4 = 33;
    private static final int PROGRESS_1_1 = 66;
    private static final int PROGRESS_9_16 = 100;

    private static final int REQUEST_RECORD = 2001;

    private TextView startRecordTxt,recordResolutionTxt,videoQualityTxt,videoRatioTxt;
    private SeekBar resolution, videoQualityBar,videoRatio;
    private EditText minDurationEt,maxDurationEt,gopEt;
    private ImageView backBtn;

    private int resolutionMode,ratioMode;
    private VideoQuality videoQuality;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_recorder_setting);
        initView();
        initAssetPath();
        copyAssets();
    }

    private void initAssetPath(){
        String path = StorageUtils.getCacheDirectory(this).getAbsolutePath() + File.separator+ Common.QU_NAME + File.separator;
        eff_dirs = new String[]{
                null,
                path + "filter/chihuang",
                path + "filter/fentao",
                path + "filter/hailan",
                path + "filter/hongrun",
                path + "filter/huibai",
                path + "filter/jingdian",
                path + "filter/maicha",
                path + "filter/nonglie",
                path + "filter/rourou",
                path + "filter/shanyao",
                path + "filter/xianguo",
                path + "filter/xueli",
                path + "filter/yangguang",
                path + "filter/youya",
                path + "filter/zhaoyang"
        };
    }



    private void copyAssets() {
        new AsyncTask() {

            @Override
            protected Object doInBackground(Object[] params) {
                Common.copyAll(RecorderSettingTest.this);
                return null;
            }

            @Override
            protected void onPostExecute(Object o) {
                startRecordTxt.setEnabled(true);
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void initView(){
        minDurationEt = (EditText) findViewById(R.id.aliyun_min_duration_edit);
        maxDurationEt = (EditText) findViewById(R.id.aliyun_max_duration_edit);
        gopEt = (EditText)findViewById(R.id.aliyun_gop_edit);
        startRecordTxt = (TextView)findViewById(R.id.aliyun_start_record);
        startRecordTxt.setOnClickListener(this);
        startRecordTxt.setEnabled(false);
        backBtn = (ImageView) findViewById(R.id.aliyun_back);
        backBtn.setOnClickListener(this);
        recordResolutionTxt = (TextView) findViewById(R.id.aliyun_resolution_txt);
        videoQualityTxt = (TextView) findViewById(R.id.aliyun_quality_txt);
        videoRatioTxt = (TextView) findViewById(R.id.aliyun_ratio_txt);
        resolution = (SeekBar) findViewById(R.id.aliyun_resolution_seekbar);
        videoQualityBar = (SeekBar) findViewById(R.id.aliyun_quality_seekbar);
        videoRatio = (SeekBar) findViewById(R.id.aliyun_ratio_seekbar);
        resolution.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if(progress <= PROGRESS_360P){
                    resolutionMode = AliyunSnapVideoParam.RESOLUTION_360P;
                    recordResolutionTxt.setText(R.string.aliyun_record_resolution_360p);
                }else if(progress > PROGRESS_360P && progress <= PROGRESS_480P){
                    resolutionMode = AliyunSnapVideoParam.RESOLUTION_480P;
                    recordResolutionTxt.setText(R.string.aliyun_record_resolution_480p);
                }else if(progress > PROGRESS_480P && progress <= PROGRESS_540P){
                    resolutionMode = AliyunSnapVideoParam.RESOLUTION_540P;
                    recordResolutionTxt.setText(R.string.aliyun_record_resolution_540p);
                }else if(progress > PROGRESS_540P){
                    resolutionMode = AliyunSnapVideoParam.RESOLUTION_720P;
                    recordResolutionTxt.setText(R.string.aliyun_record_resolution_720p);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                if(progress < PROGRESS_360P){
                    seekBar.setProgress(PROGRESS_360P);
                } else if (progress > PROGRESS_360P && progress <= PROGRESS_480P) {
                    seekBar.setProgress(PROGRESS_480P);
                } else if(progress > PROGRESS_480P && progress <= PROGRESS_540P){
                    seekBar.setProgress(PROGRESS_540P);
                } else if(progress > PROGRESS_540P){
                    seekBar.setProgress(PROGRESS_720P);
                }
            }
        });
        videoQualityBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if(progress <= PROGRESS_LOW){
                    videoQuality = VideoQuality.LD;
                    videoQualityTxt.setText(R.string.aliyun_video_quality_low);
                }else if(progress > PROGRESS_LOW && progress <= PROGRESS_MEIDAN){
                    videoQuality = VideoQuality.SD;
                    videoQualityTxt.setText(R.string.aliyun_video_quality_meidan);
                }else if(progress > PROGRESS_MEIDAN && progress <= PROGRESS_HIGH){
                    videoQuality = VideoQuality.HD;
                    videoQualityTxt.setText(R.string.aliyun_video_quality_high);
                }else if(progress > PROGRESS_HIGH){
                    videoQuality = VideoQuality.SSD;
                    videoQualityTxt.setText(R.string.aliyun_video_quality_super);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                if(progress <= PROGRESS_LOW){
                    seekBar.setProgress(PROGRESS_LOW);
                }else if(progress > PROGRESS_LOW && progress <= PROGRESS_MEIDAN){
                    seekBar.setProgress(PROGRESS_MEIDAN);
                }else if(progress > PROGRESS_MEIDAN && progress <= PROGRESS_HIGH){
                    seekBar.setProgress(PROGRESS_HIGH);
                }else if(progress > PROGRESS_HIGH){
                    seekBar.setProgress(PROGRESS_SUPER);
                }
            }
        });
        videoRatio.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if(progress <= PROGRESS_3_4){
                    ratioMode = AliyunSnapVideoParam.RATIO_MODE_3_4;
                    videoRatioTxt.setText(R.string.aliyun_record_ratio_3_4);
                }else if(progress > PROGRESS_3_4 && progress <= PROGRESS_1_1){
                    ratioMode = AliyunSnapVideoParam.RATIO_MODE_1_1;
                    videoRatioTxt.setText(R.string.aliyun_record_ratio_1_1);
                }else if(progress > PROGRESS_1_1 && progress <= PROGRESS_9_16){
                    ratioMode = AliyunSnapVideoParam.RATIO_MODE_9_16;
                    videoRatioTxt.setText(R.string.aliyun_reocrd_ratio_9_16);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int progress = seekBar.getProgress();
                if(progress < PROGRESS_3_4){
                    seekBar.setProgress(PROGRESS_3_4);
                } else if (progress > PROGRESS_3_4 && progress <= PROGRESS_1_1) {
                    seekBar.setProgress(PROGRESS_1_1);
                } else if(progress > PROGRESS_1_1 && progress <= PROGRESS_9_16){
                    seekBar.setProgress(PROGRESS_9_16);
                }
            }
        });
        resolution.setProgress(PROGRESS_540P);
        videoQualityBar.setProgress(PROGRESS_HIGH);
        videoRatio.setProgress(PROGRESS_3_4);
    }
    @Override
    public void onClick(View v) {
        if(v == startRecordTxt){
            int min = 2000;
            int max = 30000;
            int gop = 5;
            if(minDurationEt.getText() != null && !minDurationEt.getText().toString().isEmpty()){
                try {
                    min = Integer.parseInt(minDurationEt.getText().toString()) * 1000;
                }catch (Exception e){
                    Log.e("ERROR","input error");
                }
            }
            if(maxDurationEt.getText() != null && !maxDurationEt.getText().toString().isEmpty()){
                try {
                    max = Integer.parseInt(maxDurationEt.getText().toString()) * 1000;
                }catch (Exception e){
                    Log.e("ERROR","input error");
                }
            }
            if(gopEt.getText() != null && !gopEt.getText().toString().isEmpty()){
                try {
                    gop = Integer.parseInt(gopEt.getText().toString());
                }catch (Exception e){
                    Log.e("ERROR","input error");
                }
            }
            AliyunSnapVideoParam recordParam = new AliyunSnapVideoParam.Builder()
                    .setResulutionMode(resolutionMode)
                    .setRatioMode(ratioMode)
                    .setRecordMode(AliyunSnapVideoParam.RECORD_MODE_AUTO)
                    .setFilterList(eff_dirs)
                    .setBeautyLevel(80)
                    .setBeautyStatus(true)
                    .setCameraType(CameraType.FRONT)
                    .setFlashType(FlashType.ON)
                    .setNeedClip(true)
                    .setMaxDuration(max)
                    .setMinDuration(min)
                    .setVideQuality(videoQuality)
                    .setGop(gop)

                    /**
                     * 裁剪参数
                     */
                    .setMinVideoDuration(4000)
                    .setMaxVideoDuration(29 * 1000)
                    .setMinCropDuration(3000)
                    .setFrameRate(25)
                    .setCropMode(ScaleMode.PS)
                    .build();
            AliyunVideoRecorder.startRecord(this,recordParam);
        }else if(v == backBtn){
            finish();
        }
    }
}
