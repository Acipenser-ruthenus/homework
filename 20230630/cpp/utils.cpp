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
    // ��ȡCSV�ļ�����x��y�ֱ�ͳ��Ϊ1ά����
    // ���� x_values��y_values�����Ϳ�����
    static void readCSV(const string& filename, vector<int>& x_values, vector<int>& y_values) {
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
            }
        }

        file.close();
    }

    // ����ֱ��ͼ��������ȷ�����ı�־������0ʱ��ʾ������ȷ�����ز�ͬ����ʱ��ʾ��ͬ�Ĵ���
    // ���� x��y���������Լ�����������ֱ��ͼ��out_hist������
    // is_x = true ʱ����x����ֱ��ͼ���������y����ֱ��ͼ
    static int CalcHist(const vector<int>& x, const vector<int>& y, vector<int>& out_hist, bool is_x) {
        // ����CalcHist������ֱ��ͼͳ�ƽ���洢����
        vector<int> values;

        // is_x = true ʱ����x����ֱ��ͼ���������y����ֱ��ͼ
        if (is_x == true)
            values = x;
        else
            values = y;

        const int rangeMin = *min_element(values.begin(), values.end());   // ֱ��ͼ��Χ��Сֵ
        const int rangeMax = *max_element(values.begin(), values.end());  // ֱ��ͼ��Χ���ֵ
        const int histSize = rangeMax-rangeMin;  // ֱ��ͼ����Ŀ����

        double binWidth = static_cast<double>(rangeMax - rangeMin) / histSize; // ����ÿ����Ŀ�Ŀ��
        out_hist.resize(histSize+1);
        out_hist.assign(histSize+1, 0);

        for (int value : values) { // ������������
            if (value >= rangeMin && value <= rangeMax) { // ȷ������ֵ�ڷ�Χ��
                int bin = static_cast<int>((value - rangeMin) / binWidth); // ��������ֵ��������Ŀ����
                out_hist[bin]++; // ��Ӧ��Ŀ������1
            }
        }

        // ����ֱ��ͼͼ��
        int histWidth = 400; // ֱ��ͼͼ����
        int histHeight = 300; // ֱ��ͼͼ��߶�
        int binWidthPx = cvRound(static_cast<double>(histWidth) / histSize); // ����ÿ����Ŀ��ͼ���еĿ��
        Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0)); // ������ɫͼ��

        // �ҵ�ֱ��ͼ�е����Ƶ�Σ��Ա���й�һ��
        int maxFrequency = *max_element(out_hist.begin(), out_hist.end());

        // ����ֱ��ͼ
        for (int i = 0; i < histSize; i++) {
            int binHeight = cvRound(static_cast<double>(out_hist[i]) / maxFrequency * histHeight); // ����ÿ����Ŀ��ͼ���еĸ߶�
            rectangle(histImage, Point(i * binWidthPx, histHeight - binHeight), Point((i + 1) * binWidthPx - 1, histHeight - 1), Scalar(255, 255, 255), FILLED); // ����ֱ��ͼ��Ŀ����
        }

        // ��ʾֱ��ͼ
        namedWindow(" Histogram", WINDOW_NORMAL); // ��������
        imshow(" Histogram", histImage); // ��ʾͼ��
        return 0;

    }

    // �����˲�ͼ
    // ����ֱ��ͼͳ�ƽ�����ݺͶ�Ӧ�����������ַ���
    static void drawHistogramFliter(const vector<int>& histogram, const string& label) {
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



    // ֱ��ͼ��ֵ�˲�������������ȷ�����ı�־������0ʱ��ʾ������ȷ�����ز�ͬ����ʱ��ʾ��ͬ�Ĵ���
    // ���������ֱ��ͼͳ�ƽ�����ݣ��Լ��˲�������Ľ�����ݣ�����Ǹ�˹��
    static int MeanBlur(const vector<int>& in_hist, vector<int>& out_hist, const int kernal_size) {
        int size = in_hist.size();
        int step = int(kernal_size / 2);

        out_hist.resize(size);
        out_hist.assign(size, 0);
        // �˲�
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

    // ֱ��ͼ��ֵ�˲�������������ȷ�����ı�־������0ʱ��ʾ������ȷ�����ز�ͬ����ʱ��ʾ��ͬ�Ĵ���
    // ���������ֱ��ͼͳ�ƽ�����ݣ��Լ��˲�������Ľ�����ݣ�����Ǹ�˹��
    static int MedianBlur(const vector<int>& in_hist, vector<int>& out_hist, const int kernal_size) {
        int size = in_hist.size();
        int halfSize = kernal_size / 2;
        out_hist.resize(size);
        out_hist.assign(size, 0);

        for (int i = 0; i < size; i++) {
            vector<int> window;

            // ������������
            for (int j = i - halfSize; j <= i + halfSize; j++) {
                if (j >= 0 && j < size) {
                    window.push_back(in_hist[j]);
                }
            }

            // �Ի��������ڵ�ֵ��������
            sort(window.begin(), window.end());

            // ȡ��ֵ��Ϊ�˲����ֵ
            out_hist[i] = window[window.size() / 2];
        }

        return 0;
    }

    // ֱ��ͼ��˹�˲�����
    // ���������ֱ��ͼͳ�ƽ�����ݣ��Լ���˹�˲��������Ϊ�˲���ֱ��ͼ���
    static vector<int> GaussianBlur(const std::vector<int>& values, double sigma) {
        int size = values.size();
        std::vector<int> filteredValues(size, 0);

        // �����˹�˲���
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

        // Ӧ�ø�˹�˲�
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
        double angle = n * M_PI / 180.0; // ����ת����ת��Ϊ����
        vector<double> x_rotated_values;

        for (int i = 0; i < size; i++) {
            int x = x_values[i];
            int y = y_values[i];

            // ������ת���λ��
            x_rotated_values.push_back(static_cast<int>(x * cos(angle) - y * sin(angle)));
            //y_rotated_values.push_back(static_cast<int>(x * sin(angle) + y * cos(angle)));
        }
        return x_rotated_values;
    }

    static vector<double> rotatePointsY(const vector<int>& x_values, const vector<int>& y_values, double n) {
        int size = x_values.size();
        double angle = n * M_PI / 180.0; // ����ת����ת��Ϊ����
        vector<double> y_rotated_values;

        for (int i = 0; i < size; i++) {
            int x = x_values[i];
            int y = y_values[i];

            // ������ת���λ��
            
            y_rotated_values.push_back((x * sin(angle) + y * cos(angle)));
        }
        return y_rotated_values;
    }

    // ����ֱ��ͼ˫�������ͣ�������ȷ�����ı�־������0ʱ��ʾ������ȷ�����ز�ͬ����ʱ��ʾ��ͬ�Ĵ���
    // ���� x��y���������Լ�����������ֱ��ͼ��out_hist������
    // is_x = true ʱ����x����ֱ��ͼ���������y����ֱ��ͼ
    static int CalcHist2(const vector<double>& x, const vector<double>& y, vector<int>& out_hist, bool is_x) {
        // ����CalcHist������ֱ��ͼͳ�ƽ���洢����
        vector<double> values;

        // is_x = true ʱ����x����ֱ��ͼ���������y����ֱ��ͼ
        if (is_x == true)
            values = x;
        else
            values = y;

        double rangeMin = *min_element(values.begin(), values.end());   // ֱ��ͼ��Χ��Сֵ
        double rangeMax = *max_element(values.begin(), values.end());  // ֱ��ͼ��Χ���ֵ
        int histSize = ceil(rangeMax - rangeMin);  // ֱ��ͼ����Ŀ����

        double binWidth = (rangeMax - rangeMin) / histSize; // ����ÿ����Ŀ�Ŀ��
        out_hist.resize(histSize+1);
        out_hist.assign(histSize+1, 0);

        for (double value : values) { // ������������
            if (value >= rangeMin && value <= rangeMax) { // ȷ������ֵ�ڷ�Χ��
                int bin = ceil((value - rangeMin) / binWidth); // ��������ֵ��������Ŀ����
                out_hist[bin]++; // ��Ӧ��Ŀ������1
            }
        }

        // ����ֱ��ͼͼ��
        int histWidth = 400; // ֱ��ͼͼ����
        int histHeight = 300; // ֱ��ͼͼ��߶�
        double binWidthPx = (static_cast<double>(histWidth) / histSize); // ����ÿ����Ŀ��ͼ���еĿ��
        Mat histImage(histHeight, histWidth, CV_8UC3, Scalar(0, 0, 0)); // ������ɫͼ��

        // �ҵ�ֱ��ͼ�е����Ƶ�Σ��Ա���й�һ��
        int maxFrequency = *max_element(out_hist.begin(), out_hist.end());

        // ����ֱ��ͼ
        for (int i = 0; i < histSize; i++) {
            int binHeight = cvRound(static_cast<double>(out_hist[i]) / maxFrequency * histHeight); // ����ÿ����Ŀ��ͼ���еĸ߶�
            rectangle(histImage, Point(i * binWidthPx, histHeight - binHeight), Point((i + 1) * binWidthPx - 1, histHeight - 1), Scalar(255, 255, 255), FILLED); // ����ֱ��ͼ��Ŀ����
        }

        // ��ʾֱ��ͼ
        namedWindow(" Histogram", WINDOW_NORMAL); // ��������
        imshow(" Histogram", histImage); // ��ʾͼ��
        return 0;

    }
};

