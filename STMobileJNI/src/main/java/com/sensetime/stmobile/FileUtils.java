package com.sensetime.stmobile;

import android.content.Context;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;



public class FileUtils {

    private static final String FACE_TRACK_MODEL_NAME = "face_track_3.0.0.model";
    private static final String FACE_ATTRIBUTE_NAME = "face_attribute_1.0.1.model";

    public static ArrayList<String> copyStickerFiles(Context context) {
        String files[] = null;
        ArrayList<String> zipfiles = new ArrayList<String>();

        try {
            files = context.getAssets().list("");
        } catch (IOException e) {
            e.printStackTrace();
        }

        String folderpath = null;
        File dataDir = context.getExternalFilesDir(null);
        if (dataDir != null) {
            folderpath = dataDir.getAbsolutePath();
        }
        for (int i = 0; i < files.length; i++) {
            String str = files[i];
            if(str.indexOf(".zip") != -1){
                copyFileIfNeed(context, str);
            }
        }

        File file = new File(folderpath);
        File[] subFile = file.listFiles();

        for (int i = 0; i < subFile.length; i++) {
            // 判断是否为文件夹
            if (!subFile[i].isDirectory()) {
                String filename = subFile[i].getAbsolutePath();
                String path = subFile[i].getPath();
                // 判断是否为zip结尾
                if (filename.trim().toLowerCase().endsWith(".zip")) {
                    zipfiles.add(filename);
                }
            }
        }

        return zipfiles;
    }

    public static boolean copyFileIfNeed(Context context, String fileName) {
        String path = getFilePath(context, fileName);
        if (path != null) {
            File file = new File(path);
            if (!file.exists()) {
                //如果模型文件不存在
                try {
                    if (file.exists())
                        file.delete();

                    file.createNewFile();
                    InputStream in = context.getApplicationContext().getAssets().open(fileName);
                    if(in == null)
                    {
                        LogUtils.e("copyMode", "the src is not existed");
                        return false;
                    }
                    OutputStream out = new FileOutputStream(file);
                    byte[] buffer = new byte[4096];
                    int n;
                    while ((n = in.read(buffer)) > 0) {
                        out.write(buffer, 0, n);
                    }
                    in.close();
                    out.close();
                } catch (IOException e) {
                    file.delete();
                    return false;
                }
            }
        }
        return true;
    }

    public static String getFilePath(Context context, String fileName) {
        String path = null;
        File dataFile = new File(context.getApplicationContext().getFileStreamPath(fileName).getPath());
//        if (dataDir != null) {
//            path = dataDir.getAbsolutePath() + File.separator + fileName;
//        }
        return dataFile.getPath();
    }

//    public static List<Bitmap> getStickerImage(Context context) {
//        List<Bitmap> stickerList = new ArrayList<>();
//
//        Bitmap icon0 = BitmapFactory.decodeResource(context.getResources(), R.drawable.none);
//        Bitmap icon1 = BitmapFactory.decodeResource(context.getResources(), R.drawable.bunny);
//        Bitmap icon2 = BitmapFactory.decodeResource(context.getResources(), R.drawable.rabbiteating);
//
//        stickerList.add(icon0);
//        stickerList.add(icon1);
//        stickerList.add(icon2);
//
//        return stickerList;
//    }

    public static File getOutputMediaFile() {
        File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(
                Environment.DIRECTORY_DCIM), "Camera");
        // This location works best if you want the created images to be shared
        // between applications and persist after your app has been uninstalled.

        // Create the storage directory if it does not exist
        if (!mediaStorageDir.exists()) {
            if (!mediaStorageDir.mkdirs()) {
                LogUtils.e("FileUtil", "failed to create directory");
                return null;
            }
        }

        // Create a media file name
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss", Locale.CHINESE).format(new Date());
        File mediaFile = new File(mediaStorageDir.getPath() + File.separator +
                "IMG_" + timeStamp + ".jpg");

        return mediaFile;
    }

    public static void copyModelFiles(Context context) {
        copyFileIfNeed(context, FACE_TRACK_MODEL_NAME);
        copyFileIfNeed(context, FACE_ATTRIBUTE_NAME);
    }

    public static String getTrackModelPath(Context context) {
        return getFilePath(context, FACE_TRACK_MODEL_NAME);

    }

    public static String getFaceAttributeModelPath(Context context) {
        return getFilePath(context, FACE_ATTRIBUTE_NAME);
    }
}
