#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <opencv2/opencv.hpp>
#define M_PI 3.14159265358979323846

using namespace std;
using namespace cv;


class Utils{
public:
    // 读取CSV文件，将x和y分别统计为1维数组
    // 输入 x_values、y_values整数型空向量
    static void readCSV(const string& filename, vector<int>& x_values, vector<int>& y_values) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Failed to open the file." << endl;
            return;
        }

        string line;
        getline(file, line); // 读取第一行并跳过，这里假设第一行是列名
        while (getline(file, line)) { // 逐行读取文件内容
            stringstream ss(line);
            string token;
            vector<string> tokens;

            while (getline(ss, token, ',')) { // 将每行按逗号分隔为多个字段
                tokens.push_back(token);
            }

            if (tokens.size() >= 3) { // 确保每行至少有3个字段（x，y，type）
                int x = stoi(tokens[0]);
                int y = stoi(tokens[1]);

                x_values.push_back(x); // 将x值添加到x_values数组
                y_values.push_back(y); // 将y值添加到y_values数组
            }
        }

        file.close();
    }

    // 计算直方图，返回正确或错误的标志，返回0时表示计算正确，返回不同负数时表示不同的错误
    // 输入 x、y坐标数组以及其他参数，直方图从out_hist传出。
    // is_x = true 时计算x方向直方图，否则计算y方向直方图
    static int CalcHist(const vector<int>& x, const vector<int>& y, vector<int>& out_hist, bool is_x) {
        // 建立CalcHist函数内直方图统计结果存储向量
        vector<int> values;

        // is_x = true 时计算x方向直方图，否则计算y方向直方图
        if (is_x == true)
            values = x;
        else
            values = y;

        const int rangeMin = *min_element(values.begin(), values.end());   // 直方图范围最小值
        const int rangeMax = *max_element(values.begin(), values.end());  // 直方图范围最大值
        const int histSize = rangeMax-rangeMin;  // 直方图的条目数量

        double binWidth = static_cast<double>(rangeMax - rangeMin) / histSize; // 计算每个条目的宽度
        out_hist.resize(histSize+1);
        out_hist.assign(histSize+1, 0);

        for (int value : values) { // 遍历数据数组
            if (value >= rangeMin && value <= rangeMax) { // 确保数据值在范围内
                int bin = static_cast<int>((value - rangeMin) / binWidth); // 计算数据值所属的条目索引
                out_hist[bin]++; // 相应条目计数加1
            }
        }

        // 创建直方图图像
        int histWidth = 400; // 直方图图像宽度
        int histHeight = 300; // 直方图图像高度
        int binWidthPx = cvRound(static_cast<double>(histWidth) / histSize); // 计算每个条目在图像中的宽度
        Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0)); // 创建黑色图像

        // 找到直方图中的最大频次，以便进行归一化
        int maxFrequency = *max_element(out_hist.begin(), out_hist.end());

        // 绘制直方图
        for (int i = 0; i < histSize; i++) {
            int binHeight = cvRound(static_cast<double>(out_hist[i]) / maxFrequency * histHeight); // 计算每个条目在图像中的高度
            rectangle(histImage, Point(i * binWidthPx, histHeight - binHeight), Point((i + 1) * binWidthPx - 1, histHeight - 1), Scalar(255, 255, 255), FILLED); // 绘制直方图条目矩形
        }

        // 显示直方图
        namedWindow(" Histogram", WINDOW_NORMAL); // 创建窗口
        imshow(" Histogram", histImage); // 显示图像
        return 0;

    }

    // 绘制滤波图
    // 输入直方图统计结果数据和对应的坐标轴名字符串
    static void drawHistogramFliter(const vector<int>& histogram, const string& label) {
        const int histSize = 255;  // 直方图的条目数量
        const int rangeMin = 0;   // 直方图范围最小值
        const int rangeMax = 255;  // 直方图范围最大值

        double binWidth = static_cast<double>(rangeMax - rangeMin) / histSize; // 计算每个条目的宽度


        // 创建直方图图像
        int histWidth = 400; // 直方图图像宽度
        int histHeight = 300; // 直方图图像高度
        int binWidthPx = cvRound(static_cast<double>(histWidth) / histSize); // 计算每个条目在图像中的宽度
        Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0)); // 创建黑色图像

        // 找到直方图中的最大频次，以便进行归一化
        int maxFrequency = *max_element(histogram.begin(), histogram.end());

        // 绘制直方图
        for (int i = 0; i < histSize; i++) {
            int binHeight = cvRound(static_cast<double>(histogram[i]) / maxFrequency * histHeight); // 计算每个条目在图像中的高度
            rectangle(histImage, Point(i * binWidthPx, histHeight - binHeight), Point((i + 1) * binWidthPx - 1, histHeight - 1), Scalar(255, 255, 255), FILLED); // 绘制直方图条目矩形
        }

        // 显示直方图
        namedWindow(label + " Histogram", WINDOW_NORMAL); // 创建窗口
        imshow(label + " Histogram", histImage); // 显示图像

    }



    // 直方图均值滤波函数，返回正确或错误的标志，返回0时表示计算正确，返回不同负数时表示不同的错误
    // 输入基础的直方图统计结果数据，以及滤波后输出的结果数据，最后是高斯核
    static int MeanBlur(const vector<int>& in_hist, vector<int>& out_hist, const int kernal_size) {
        int size = in_hist.size();
        int step = int(kernal_size / 2);

        out_hist.resize(size);
        out_hist.assign(size, 0);
        // 滤波
        for (int i = 0; i < size; i++) {
            int sum = 0;
            int count = 0;
            for (int j = i - step; j <= i + step; j++) {
                if (j >= 0 && j < size) {
                    sum += in_hist[j];
                    count++;
                }
            }
            out_hist[i] = sum / count;
        }

        return 0;
    }

    // 直方图中值滤波函数，返回正确或错误的标志，返回0时表示计算正确，返回不同负数时表示不同的错误
    // 输入基础的直方图统计结果数据，以及滤波后输出的结果数据，最后是高斯核
    static int MedianBlur(const vector<int>& in_hist, vector<int>& out_hist, const int kernal_size) {
        int size = in_hist.size();
        int halfSize = kernal_size / 2;
        out_hist.resize(size);
        out_hist.assign(size, 0);

        for (int i = 0; i < size; i++) {
            vector<int> window;

            // 构建滑动窗口
            for (int j = i - halfSize; j <= i + halfSize; j++) {
                if (j >= 0 && j < size) {
                    window.push_back(in_hist[j]);
                }
            }

            // 对滑动窗口内的值进行排序
            sort(window.begin(), window.end());

            // 取中值作为滤波后的值
            out_hist[i] = window[window.size() / 2];
        }

        return 0;
    }

    // 直方图高斯滤波函数
    // 输入基础的直方图统计结果数据，以及高斯核参数；输出为滤波后直方图结果
    static vector<int> GaussianBlur(const std::vector<int>& values, double sigma) {
        int size = values.size();
        std::vector<int> filteredValues(size, 0);

        // 计算高斯滤波核
        int kernelSize = 2 * ceil(2 * sigma) + 1;
        std::vector<double> kernel(kernelSize);
        double sum = 0.0;
        for (int i = 0; i < kernelSize; i++) {
            double x = i - kernelSize / 2;
            kernel[i] = exp(-0.5 * pow(x / sigma, 2));
            sum += kernel[i];
        }
        for (int i = 0; i < kernelSize; i++) {
            kernel[i] /= sum;
        }

        // 应用高斯滤波
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < kernelSize; j++) {
                int index = i + j - kernelSize / 2;
                if (index >= 0 && index < size) {
                    filteredValues[i] += static_cast<int>(values[index] * kernel[j]);
                }
            }
        }

        return filteredValues;
    }

    static vector<double> rotatePointsX(const vector<int>& x_values, const vector<int>& y_values, int n) {
        int size = x_values.size();
        double angle = n * M_PI / 180.0; // 将旋转度数转换为弧度
        vector<double> x_rotated_values;

        for (int i = 0; i < size; i++) {
            int x = x_values[i];
            int y = y_values[i];

            // 计算旋转后的位置
            x_rotated_values.push_back(static_cast<int>(x * cos(angle) - y * sin(angle)));
            //y_rotated_values.push_back(static_cast<int>(x * sin(angle) + y * cos(angle)));
        }
        return x_rotated_values;
    }

    static vector<double> rotatePointsY(const vector<int>& x_values, const vector<int>& y_values, double n) {
        int size = x_values.size();
        double angle = n * M_PI / 180.0; // 将旋转度数转换为弧度
        vector<double> y_rotated_values;

        for (int i = 0; i < size; i++) {
            int x = x_values[i];
            int y = y_values[i];

            // 计算旋转后的位置
            
            y_rotated_values.push_back((x * sin(angle) + y * cos(angle)));
        }
        return y_rotated_values;
    }

    // 计算直方图双精度类型，返回正确或错误的标志，返回0时表示计算正确，返回不同负数时表示不同的错误
    // 输入 x、y坐标数组以及其他参数，直方图从out_hist传出。
    // is_x = true 时计算x方向直方图，否则计算y方向直方图
    static int CalcHist2(const vector<double>& x, const vector<double>& y, vector<int>& out_hist, bool is_x) {
        // 建立CalcHist函数内直方图统计结果存储向量
        vector<double> values;

        // is_x = true 时计算x方向直方图，否则计算y方向直方图
        if (is_x == true)
            values = x;
        else
            values = y;

        double rangeMin = *min_element(values.begin(), values.end());   // 直方图范围最小值
        double rangeMax = *max_element(values.begin(), values.end());  // 直方图范围最大值
        int histSize = ceil(rangeMax - rangeMin);  // 直方图的条目数量

        double binWidth = (rangeMax - rangeMin) / histSize; // 计算每个条目的宽度
        out_hist.resize(histSize+1);
        out_hist.assign(histSize+1, 0);

        for (double value : values) { // 遍历数据数组
            if (value >= rangeMin && value <= rangeMax) { // 确保数据值在范围内
                int bin = ceil((value - rangeMin) / binWidth); // 计算数据值所属的条目索引
                out_hist[bin]++; // 相应条目计数加1
            }
        }

        // 创建直方图图像
        int histWidth = 400; // 直方图图像宽度
        int histHeight = 300; // 直方图图像高度
        double binWidthPx = (static_cast<double>(histWidth) / histSize); // 计算每个条目在图像中的宽度
        Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0)); // 创建黑色图像

        // 找到直方图中的最大频次，以便进行归一化
        int maxFrequency = *max_element(out_hist.begin(), out_hist.end());

        // 绘制直方图
        for (int i = 0; i < histSize; i++) {
            int binHeight = cvRound(static_cast<double>(out_hist[i]) / maxFrequency * histHeight); // 计算每个条目在图像中的高度
            rectangle(histImage, Point(i * binWidthPx, histHeight - binHeight), Point((i + 1) * binWidthPx - 1, histHeight - 1), Scalar(255, 255, 255), FILLED); // 绘制直方图条目矩形
        }

        // 显示直方图
        namedWindow(" Histogram", WINDOW_NORMAL); // 创建窗口
        imshow(" Histogram", histImage); // 显示图像
        return 0;

    }
};

