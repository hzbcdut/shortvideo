/*
 * Copyright (C) 2010-2017 Alibaba Group Holding Limited.
 */

package qupai.aliyun.qusdkdemo;

import android.app.Application;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.support.multidex.MultiDex;

import com.aliyun.common.crash.CrashHandler;
import com.aliyun.common.httpfinal.QupaiHttpFinal;
import com.aliyun.common.logger.Logger;
import com.aliyun.downloader.DownloaderManager;
import com.aliyun.log.core.AliyunLoggerManager;
import com.aliyun.nativecrash.NativeCrashHandler;

import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

/**
 * Created by yan on 2017/2/8.
 */

public class App extends Application {
    private String mSignature;
    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        MultiDex.install(this);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        loadLibs();
        QupaiHttpFinal.getInstance().initOkHttpFinal();
        Logger.setDebug(true);
        initDownLoader();
//        localCrashHandler();
//        new NativeCrashHandler().registerForNativeCrash(this);

    }

    private void loadLibs(){
        System.loadLibrary("openh264");
        System.loadLibrary("encoder");
        System.loadLibrary("AliFaceAlignmentModule");
        System.loadLibrary("aliface_jni");
        System.loadLibrary("QuCore-ThirdParty");
        System.loadLibrary("QuCore");
    }

    HostnameVerifier DO_NOT_VERIFY = new HostnameVerifier() {
        @Override
        public boolean verify(String hostname, SSLSession session) {
            return true;
        }
    };

    private void initDownLoader() {
        DownloaderManager.getInstance().init(this);
    }

    private  void trustAllHosts() {
        // Create a trust manager that does not validate certificate chains
        // Android use X509 cert
        TrustManager[] trustAllCerts = new TrustManager[] { new X509TrustManager() {
            public X509Certificate[] getAcceptedIssuers() {
                return new X509Certificate[] {};
            }

            public void checkClientTrusted(X509Certificate[] chain,
                                           String authType) throws CertificateException {
            }

            public void checkServerTrusted(X509Certificate[] chain,
                                           String authType) throws CertificateException {
            }
        } };

        // Install the all-trusting trust manager
        try {
            SSLContext sc = SSLContext.getInstance("TLS");
            sc.init(null, trustAllCerts, new java.security.SecureRandom());
            HttpsURLConnection
                    .setDefaultSSLSocketFactory(sc.getSocketFactory());
            HttpsURLConnection.setDefaultHostnameVerifier(DO_NOT_VERIFY);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void localCrashHandler() {
        CrashHandler catchHandler = CrashHandler.getInstance();
        catchHandler.init(getApplicationContext());
    }

}
