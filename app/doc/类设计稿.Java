
// 音乐播放器
public class AudioPlayer {
	// 从队列中获取解析好的数据
	Queue queue; 
	// 表示当前播放到的时间
	long cur;
    // 开始播放
	boolean startPlay();
	// 暂停播放
	boolean stop();
	// 释放资源
	boolean release();
}

// 表示资源，可以是文件，也可以是网络文件、也可以是Asset下文件
public class Resource{
	Uri path; // 表示资源的路径

    // 打开资源，得到流。
	InputStream open();

}

// 表示音频的一帧
class AudioFrame{

}

// 存放解码后的数据的队列
public class Queue {
	// 队列满了阻塞
	void put(AudioFrame frame);
	// 队列空会阻塞
	void take();
	//  清空队列
	void clear();
    // 获取队列容量
	int size();

}

public class AudioDecoder {
	// 表示需要解码的资源
	Resource res; 
    // 表示总的时间
	long total;

	// 开始解码
	boolean startDecod();
	// 停止解码
	boolean stopDecod();
	// 释放资源
	boolean release();
    // 拖动
	void seek(float progress);

}

public class AudioController {
	AudioPlayer player;
	AudioDecoder decoder;
	ProgressCallBack progressCallBack;
	// 初始化
	void init();

    // 开启定时回调
	void satrtTimerCallBack();

    // 拖动快进
	boolean seek(float progress) {
		// 定位到特定资源位置
		// 清空已有队列
		// 从特定位置开始解析

	}

    // 下面的方法委托到播放器实现
	boolean startPlay();
	// 暂停播放
	boolean stop();
	// 释放资源
	boolean release();


}

// 进度回调
public interface ProgressCallBack {
	void onProgress(float progress);
}

