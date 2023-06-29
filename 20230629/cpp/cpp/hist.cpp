#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// 读取CSV文件，将x和y分别统计为1维数组
void readCSV(const string& filename, vector<int>& x_values, vector<int>& y_values, vector<int>& z_values, vector<int>& x_histogram, vector<int>& y_histogram) {
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
            z_values.push_back(x); // 将x值添加到z_values数组
            z_values.push_back(y); // 将y值添加到z_values数组

            // 统计x的直方图数据
            if (x >= 0 && x < x_histogram.size()) {
                x_histogram[x]++;
            }

            // 统计y的直方图数据
            if (y >= 0 && y < y_histogram.size()) {
                y_histogram[y]++;
            }
        }
    }

    file.close();
}

// 绘制直方图
void drawHistogram(const vector<int>& values, const string& label) {
    const int histSize = 255;  // 直方图的条目数量
    const int rangeMin = 0;   // 直方图范围最小值
    const int rangeMax = 255;  // 直方图范围最大值

    vector<int> histogram(histSize, 0); // 存储直方图数据的数组，初始值为0
    double binWidth = static_cast<double>(rangeMax - rangeMin) / histSize; // 计算每个条目的宽度

    for (int value : values) { // 遍历数据数组
        if (value >= rangeMin && value <= rangeMax) { // 确保数据值在范围内
            int bin = static_cast<int>((value - rangeMin) / binWidth); // 计算数据值所属的条目索引
            histogram[bin]++; // 相应条目计数加1
        }
    }

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

// 绘制滤波图
void drawHistogramFliter(const vector<int>& histogram, const string& label) {
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



// 直方图均值滤波函数
vector<int> meanFilter(const vector<int>& values, int step) {
    int size = values.size();
    step = int(step / 2);
    
    vector<int> filteredValues(size, 0);

    // 滤波
    for (int i = 0; i < size; i++) {
        int sum = 0;
        int count = 0;
        for (int j = i - step; j <= i + step; j++) {
            if (j >= 0 && j < size) {
                sum += values[j];
                count++;
            }
        }
        filteredValues[i] = sum / count;
    }

    return filteredValues;
}

// 直方图中值滤波函数
vector<int> medianFilter(const vector<int>& values, int windowSize) {
    int size = values.size();
    int halfSize = windowSize / 2;
    vector<int> filteredValues(size, 0);

    for (int i = 0; i < size; i++) {
        vector<int> window;

        // 构建滑动窗口
        for (int j = i - halfSize; j <= i + halfSize; j++) {
            if (j >= 0 && j < size) {
                window.push_back(values[j]);
            }
        }

        // 对滑动窗口内的值进行排序
        sort(window.begin(), window.end());

        // 取中值作为滤波后的值
        filteredValues[i] = window[window.size() / 2];
    }

    return filteredValues;
}

int main() {
    vector<int> x_values;
    vector<int> y_values;
    vector<int> z_values;
    vector<int> x_histogram(255,0);
    vector<int> y_histogram(255,0);


    readCSV("C:/Users/U/Desktop/1(1).csv", x_values, y_values, z_values, x_histogram, y_histogram); // 读取CSV文件

    drawHistogram(x_values, "x"); // 绘制x的直方图
    //drawHistogram(y_values, "y"); // 绘制y的直方图
    //drawHistogram(z_values, "z"); // 绘制z的直方图

    int step = 3;
    //sort(x_valueshist.begin(), x_valueshist.end());
    vector<int> filteredValuesXMean = meanFilter(x_histogram, step);
    
    vector<int> filteredValuesXMedian = medianFilter(x_histogram, step);

    vector<int> filteredValuesXMMM1 = medianFilter(x_histogram, 3);
    vector<int> filteredValuesXMMM2 = meanFilter(filteredValuesXMMM1, 3);
    vector<int> filteredValuesXMMM3 = meanFilter(filteredValuesXMMM2, 3);

    //drawHistogramFliter(filteredValuesXMean, "x_Mean_step_3"); // 绘制均值滤波的直方图
    //drawHistogramFliter(filteredValuesXMedian, "x_Median_step_3_M"); // 绘制中值滤波的直方图
    drawHistogramFliter(filteredValuesXMMM3, "x_MMM"); // 绘制1次中值2次均值滤波后的直方图


    waitKey(0);

    return 0;
}


//int main() {
//
//    vector<int> x_values;
//    vector<int> y_values;
//    vector<int> z_values;
//    vector<int> x_histogram(255, 0);
//    vector<int> y_histogram(255, 0);
//
//
//    readCSV("C:/Users/U/Desktop/1(1).csv", x_values, y_values, z_values, x_histogram, y_histogram); // 读取CSV文件
//
//    // 设置不同的滤波步长
//    vector<int> filterSteps = {1, 3, 5, 7, 9 };
//    int numSteps = filterSteps.size();
//
//    // 创建大图
//    int imageSize = 200;
//    int histSize = 10;
//    int histHeight = 100;
//    Mat bigImage(histHeight * numSteps, imageSize * histSize, CV_8UC3, Scalar(0, 0, 0));
//
//    // 绘制子图
//    for (int i = 0; i < numSteps; i++) {
//        int step = filterSteps[i];
//
//        // 进行滤波
//        vector<int> filteredValuesX = histogramValueFilter(x_histogram, step);
//
//        // 创建子图
//        Mat histImage(histHeight, imageSize, CV_8UC3, Scalar(0, 0, 0));
//
//        // 绘制x的直方图滤波
//        drawHistogramFliter(filteredValuesX, "x_filtered_step_3"); 
//
//        // 将子图放入大图中
//        Rect roi(i * imageSize, 0, imageSize, histHeight);
//        Mat subImage = bigImage(roi);
//        histImage.copyTo(subImage);
//    }
//
//    // 显示大图
//    namedWindow("Histograms", WINDOW_NORMAL);
//    imshow("Histograms", bigImage);
//
//    waitKey(0);
//
//    return 0;
//}