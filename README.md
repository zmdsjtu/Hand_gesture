# 手势识别与分割

- 基于fhog特征的手势提取和识别

- 基于手势的bouding box和类别进行特征点定位达到分割的目的
- 基于MedianFlow的追踪算法保证boudingbox追踪的稳定性

# Requirements

- [OpenCV3.1 + contribute](http://opencv.org/downloads.html)

- [dlib19.9](http://dlib.net/)




## 程序运行展示

- 运行程序的主界面


![Demo](/snapshot/main2.png)

- 程序视频演示demo

[视频Demo](/snapshot/video_demo.avi)

- 特征点定位效果
手掌
![palm](/snapshot/palm.png)
拳头
![fist](/snapshot/fist.png)
胜利
![scissor](/snapshot/scissor.png)
点赞
![thumb](/snapshot/thumb.png)

## 程序操作说明

- 控制台输入视频所在目录
- 如果输入数字打开对应编号的摄像头
- ESC退出

## 程序设置说明
- Threshold: 调整检测到各种手势的阈值
- 2X、3X、4X: 不同图片尺度以减少图片检测的时间
- show time: 是否显示每张图片所用时间
- Bouding: 是否显示bouding box
- Recognition: 是否显示检测结果
- Segmentation: 是否显示手势分割结果（左下角）
- Points: 是否显示手势的特征点

## 程序流程
如流程图所示
- 程序开始时基于fHog特征金字塔对多种手势模型进行匹配，得到分值最高的手势和所在的bouding box
- 将当前的box和图像初始化MedianFlow追踪算法
- 追踪算法得到的图像所在的bouding box输出到特征点回归模块得到特征点
- 特征点为各手势的轮廓点，继而可以分割出手势所在区域
- 追踪失败时，重新利用检测的结果初始追踪算法
- 此外，当检测模块和追踪算法的识别结果不同或者bouding box重合区域小于70%时判定追踪失败
![Flowchart](/snapshot/flowchart.png)

## 算法特点
- fHog特征可以很好地表征手的轮廓特征
- 金字塔解决了检测多尺度的问题
- 检测分值可以甄别手势
- 图片缩放可以一定程度上减轻遍历图片费时的问题
- MedianFlow算法可以一定程度上判断出追踪丢失或者失败
- MedianFlow算法效率较高，不缩放的图片单帧追踪时间量级是10ms
- 基于bouding box的特征点回归可以定位到各手势特定位置，诸如单个指尖、掌心等等
- 基于特征点的分割相较于基于肤色的分割可以更鲁棒地应对光照条件


## Reference

1. [dlib环境配置](http://blog.csdn.net/zmdsjtu/article/details/53454071)

2. [OpenCV contribute环境配置](http://blog.csdn.net/zmdsjtu/article/details/78069739)

3. Object detection with discriminatively trained partbased models. IEEE Trans. PAMI, 32(9):1627–1645, 2010.（手势定位与识别，fHog特征）
4. One Millisecond Face Alignment with an Ensemble of Regression Trees by Vahid Kazemi and Josephine Sullivan, CVPR 2014.（特征点定位）
5. [Opencv3_contribute追踪算法封装](https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/)