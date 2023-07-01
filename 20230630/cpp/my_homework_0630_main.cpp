#include <vector>
#include "utils.cpp"

using namespace std;

int main() {
    vector<int> x_values;
    vector<int> y_values;

    Utils::readCSV("C:/Users/U/Desktop/1(1).csv", x_values, y_values); // ��ȡCSV�ļ�


    //// x��ֱ��ͼ
    //vector<int> out_hist_x(255, 0); // �洢ֱ��ͼ���ݵ����飬��ʼֵΪ0
    //Utils::CalcHist(x_values, y_values, out_hist_x, 1); // ����x��ֱ��ͼ

    //vector<int> out_hist1_x(255, 0); // �洢�˲�ֱ��ͼ���ݵ����飬��ʼֵΪ0
    //Utils::MedianBlur(out_hist_x, out_hist1_x, 3); //��һ���˲�
    //Utils::MeanBlur(out_hist1_x, out_hist_x, 3); // �ڶ����˲�
    //Utils::MeanBlur(out_hist_x, out_hist1_x, 3); // �������˲�

    //Utils::drawHistogramFliter(out_hist1_x, "MMM"); // ����1����ֵ2�ξ�ֵ�˲����ֱ��ͼ

    // y��ֱ��ͼ
    vector<int> out_hist_y(255,0); // �洢ֱ��ͼ���ݵ����飬��ʼֵΪ0
    Utils::CalcHist(x_values, y_values, out_hist_y, 0); // ����y��ֱ��ͼ

    vector<int> out_hist1_y(255,0); // �洢�˲�ֱ��ͼ���ݵ����飬��ʼֵΪ0
    Utils::MedianBlur(out_hist_y, out_hist1_y, 3); //��һ���˲�
    Utils::MeanBlur(out_hist1_y, out_hist_y, 3); // �ڶ����˲�
    Utils::MeanBlur(out_hist_y, out_hist1_y, 3); // �������˲�
    //Utils::drawHistogramFliter(out_hist1_y, "MMM"); // ����1����ֵ2�ξ�ֵ�˲����ֱ��ͼ


    // ɢ��ͼ��ת
    // ��ת�Ƕ�
    double angle = 45;

    // x����תֱ��ͼ
    vector<double> in_hist_x_rotated = Utils::rotatePointsX(x_values, y_values, angle);
    // Utils::CalcHist(x_rotated_values, y_rotated_values, out_hist_x_rotated, 1); // ����x��ֱ��ͼ

    // y����תֱ��ͼ
    vector<double> in_hist_y_rotated = Utils::rotatePointsY(x_values, y_values, angle);
    vector<int> out_hist_y_rotated;
    Utils::CalcHist2(in_hist_x_rotated, in_hist_y_rotated, out_hist_y_rotated, 0); // ����x��ֱ��ͼ



    waitKey(0);

    return 0;
}
