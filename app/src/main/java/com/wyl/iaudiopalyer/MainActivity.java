package com.wyl.iaudiopalyer;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.wyl.iaudiopalyer.databinding.ActivityMainBinding;

import java.io.File;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    public static final String TAG = MainActivity.class.getSimpleName();

    // Used to load the 'iaudiopalyer' library on application startup.
    static {
        System.loadLibrary("iaudiopalyer");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
//        AudioMetadata audioMetadata = new AudioMetadata(getCachedMedia(".mp3"));
//        audioMetadata.parse();
        Log.d(TAG, "onCreate: ");

        startReadPcm(getCachedMedia(".mp3"), getSavePcmPath());
    }

    private String getSavePcmPath() {
        File file = new File(getFilesDir(), "解码后音频数据.pcm");
        if (file.exists()) {
            file.delete();
        }
        try {
            file.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return file.getAbsolutePath();
    }

    private String getCachedMedia(String suffix) {
        File[] files = getFilesDir().listFiles();
        if (files != null) {
            for (File file : files) {
                if (file.getName().endsWith(suffix)) {
                    return file.getAbsolutePath();
                }
            }
        }
        return "";
    }

    /**
     * A native method that is implemented by the 'iaudiopalyer' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public static native void startReadPcm(String path, String savePath);
}