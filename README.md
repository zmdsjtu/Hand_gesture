# 手势识别与分割

- 基于fhog特征的手势提取和识别

- 基于手势的bouding box和类别进行特征点定位达到分割的目的

# Requirements

- [OpenCV3.1 + contribute](http://opencv.org/downloads.html)

- [dlib19.9](http://dlib.net/)




## 程序运行展示

- 运行程序的主界面



![Demo](/snapshot/main.png)


## Reference

1. [dlib环境配置](http://blog.csdn.net/zmdsjtu/article/details/53454071)

2. [OpenCV contribute环境配置](http://blog.csdn.net/zmdsjtu/article/details/78069739)

3. Object detection with discriminatively trained partbased models. IEEE Trans. PAMI, 32(9):1627–1645, 2010.（手势定位与识别，fHog特征）
4. One Millisecond Face Alignment with an Ensemble of Regression Trees by Vahid Kazemi and Josephine Sullivan, CVPR 2014.（特征点定位）