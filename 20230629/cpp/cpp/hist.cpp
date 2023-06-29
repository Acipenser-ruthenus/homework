#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// ��ȡCSV�ļ�����x��y�ֱ�ͳ��Ϊ1ά����
void readCSV(const string& filename, vector<int>& x_values, vector<int>& y_values, vector<int>& z_values, vector<int>& x_histogram, vector<int>& y_histogram) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open the file." << endl;
        return;
    }

    string line;
    getline(file, line); // ��ȡ��һ�в���������������һ��������
    while (getline(file, line)) { // ���ж�ȡ�ļ�����
        stringstream ss(line);
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) { // ��ÿ�а����ŷָ�Ϊ����ֶ�
            tokens.push_back(token);
        }

        if (tokens.size() >= 3) { // ȷ��ÿ��������3���ֶΣ�x��y��type��
            int x = stoi(tokens[0]);
            int y = stoi(tokens[1]);

            x_values.push_back(x); // ��xֵ��ӵ�x_values����
            y_values.push_back(y); // ��yֵ��ӵ�y_values����
            z_values.push_back(x); // ��xֵ��ӵ�z_values����
            z_values.push_back(y); // ��yֵ��ӵ�z_values����

            // ͳ��x��ֱ��ͼ����
            if (x >= 0 && x < x_histogram.size()) {
                x_histogram[x]++;
            }

            // ͳ��y��ֱ��ͼ����
            if (y >= 0 && y < y_histogram.size()) {
                y_histogram[y]++;
            }
        }
    }

    file.close();
}

// ����ֱ��ͼ
void drawHistogram(const vector<int>& values, const string& label) {
    const int histSize = 255;  // ֱ��ͼ����Ŀ����
    const int rangeMin = 0;   // ֱ��ͼ��Χ��Сֵ
    const int rangeMax = 255;  // ֱ��ͼ��Χ���ֵ

    vector<int> histogram(histSize, 0); // �洢ֱ��ͼ���ݵ����飬��ʼֵΪ0
    double binWidth = static_cast<double>(rangeMax - rangeMin) / histSize; // ����ÿ����Ŀ�Ŀ��

    for (int value : values) { // ������������
        if (value >= rangeMin && value <= rangeMax) { // ȷ������ֵ�ڷ�Χ��
            int bin = static_cast<int>((value - rangeMin) / binWidth); // ��������ֵ��������Ŀ����
            histogram[bin]++; // ��Ӧ��Ŀ������1
        }
    }

    // ����ֱ��ͼͼ��
    int histWidth = 400; // ֱ��ͼͼ����
    int histHeight = 300; // ֱ��ͼͼ��߶�
    int binWidthPx = cvRound(static_cast<double>(histWidth) / histSize); // ����ÿ����Ŀ��ͼ���еĿ��
    Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0)); // ������ɫͼ��

    // �ҵ�ֱ��ͼ�е����Ƶ�Σ��Ա���й�һ��
    int maxFrequency = *max_element(histogram.begin(), histogram.end());

    // ����ֱ��ͼ
    for (int i = 0; i < histSize; i++) {
        int binHeight = cvRound(static_cast<double>(histogram[i]) / maxFrequency * histHeight); // ����ÿ����Ŀ��ͼ���еĸ߶�
        rectangle(histImage, Point(i * binWidthPx, histHeight - binHeight), Point((i + 1) * binWidthPx - 1, histHeight - 1), Scalar(255, 255, 255), FILLED); // ����ֱ��ͼ��Ŀ����
    }

    // ��ʾֱ��ͼ
    namedWindow(label + " Histogram", WINDOW_NORMAL); // ��������
    imshow(label + " Histogram", histImage); // ��ʾͼ��

}

// �����˲�ͼ
void drawHistogramFliter(const vector<int>& histogram, const string& label) {
    const int histSize = 255;  // ֱ��ͼ����Ŀ����
    const int rangeMin = 0;   // ֱ��ͼ��Χ��Сֵ
    const int rangeMax = 255;  // ֱ��ͼ��Χ���ֵ

    double binWidth = static_cast<double>(rangeMax - rangeMin) / histSize; // ����ÿ����Ŀ�Ŀ��


    // ����ֱ��ͼͼ��
    int histWidth = 400; // ֱ��ͼͼ����
    int histHeight = 300; // ֱ��ͼͼ��߶�
    int binWidthPx = cvRound(static_cast<double>(histWidth) / histSize); // ����ÿ����Ŀ��ͼ���еĿ��
    Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0)); // ������ɫͼ��

    // �ҵ�ֱ��ͼ�е����Ƶ�Σ��Ա���й�һ��
    int maxFrequency = *max_element(histogram.begin(), histogram.end());

    // ����ֱ��ͼ
    for (int i = 0; i < histSize; i++) {
        int binHeight = cvRound(static_cast<double>(histogram[i]) / maxFrequency * histHeight); // ����ÿ����Ŀ��ͼ���еĸ߶�
        rectangle(histImage, Point(i * binWidthPx, histHeight - binHeight), Point((i + 1) * binWidthPx - 1, histHeight - 1), Scalar(255, 255, 255), FILLED); // ����ֱ��ͼ��Ŀ����
    }

    // ��ʾֱ��ͼ
    namedWindow(label + " Histogram", WINDOW_NORMAL); // ��������
    imshow(label + " Histogram", histImage); // ��ʾͼ��

}



// ֱ��ͼ��ֵ�˲�����
vector<int> meanFilter(const vector<int>& values, int step) {
    int size = values.size();
    step = int(step / 2);
    
    vector<int> filteredValues(size, 0);

    // �˲�
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

// ֱ��ͼ��ֵ�˲�����
vector<int> medianFilter(const vector<int>& values, int windowSize) {
    int size = values.size();
    int halfSize = windowSize / 2;
    vector<int> filteredValues(size, 0);

    for (int i = 0; i < size; i++) {
        vector<int> window;

        // ������������
        for (int j = i - halfSize; j <= i + halfSize; j++) {
            if (j >= 0 && j < size) {
                window.push_back(values[j]);
            }
        }

        // �Ի��������ڵ�ֵ��������
        sort(window.begin(), window.end());

        // ȡ��ֵ��Ϊ�˲����ֵ
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


    readCSV("C:/Users/U/Desktop/1(1).csv", x_values, y_values, z_values, x_histogram, y_histogram); // ��ȡCSV�ļ�

    drawHistogram(x_values, "x"); // ����x��ֱ��ͼ
    //drawHistogram(y_values, "y"); // ����y��ֱ��ͼ
    //drawHistogram(z_values, "z"); // ����z��ֱ��ͼ

    int step = 3;
    //sort(x_valueshist.begin(), x_valueshist.end());
    vector<int> filteredValuesXMean = meanFilter(x_histogram, step);
    
    vector<int> filteredValuesXMedian = medianFilter(x_histogram, step);

    vector<int> filteredValuesXMMM1 = medianFilter(x_histogram, 3);
    vector<int> filteredValuesXMMM2 = meanFilter(filteredValuesXMMM1, 3);
    vector<int> filteredValuesXMMM3 = meanFilter(filteredValuesXMMM2, 3);

    //drawHistogramFliter(filteredValuesXMean, "x_Mean_step_3"); // ���ƾ�ֵ�˲���ֱ��ͼ
    //drawHistogramFliter(filteredValuesXMedian, "x_Median_step_3_M"); // ������ֵ�˲���ֱ��ͼ
    drawHistogramFliter(filteredValuesXMMM3, "x_MMM"); // ����1����ֵ2�ξ�ֵ�˲����ֱ��ͼ


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
//    readCSV("C:/Users/U/Desktop/1(1).csv", x_values, y_values, z_values, x_histogram, y_histogram); // ��ȡCSV�ļ�
//
//    // ���ò�ͬ���˲�����
//    vector<int> filterSteps = {1, 3, 5, 7, 9 };
//    int numSteps = filterSteps.size();
//
//    // ������ͼ
//    int imageSize = 200;
//    int histSize = 10;
//    int histHeight = 100;
//    Mat bigImage(histHeight * numSteps, imageSize * histSize, CV_8UC3, Scalar(0, 0, 0));
//
//    // ������ͼ
//    for (int i = 0; i < numSteps; i++) {
//        int step = filterSteps[i];
//
//        // �����˲�
//        vector<int> filteredValuesX = histogramValueFilter(x_histogram, step);
//
//        // ������ͼ
//        Mat histImage(histHeight, imageSize, CV_8UC3, Scalar(0, 0, 0));
//
//        // ����x��ֱ��ͼ�˲�
//        drawHistogramFliter(filteredValuesX, "x_filtered_step_3"); 
//
//        // ����ͼ�����ͼ��
//        Rect roi(i * imageSize, 0, imageSize, histHeight);
//        Mat subImage = bigImage(roi);
//        histImage.copyTo(subImage);
//    }
//
//    // ��ʾ��ͼ
//    namedWindow("Histograms", WINDOW_NORMAL);
//    imshow("Histograms", bigImage);
//
//    waitKey(0);
//
//    return 0;
//}