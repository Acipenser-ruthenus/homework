#include <vector>
#include "utils.cpp"

using namespace std;

int main() {
    vector<int> x_values;
    vector<int> y_values;

    Utils::readCSV("C:/Users/U/Desktop/1(1).csv", x_values, y_values); // 读取CSV文件


    //// x轴直方图
    //vector<int> out_hist_x(255, 0); // 存储直方图数据的数组，初始值为0
    //Utils::CalcHist(x_values, y_values, out_hist_x, 1); // 绘制x的直方图

    //vector<int> out_hist1_x(255, 0); // 存储滤波直方图数据的数组，初始值为0
    //Utils::MedianBlur(out_hist_x, out_hist1_x, 3); //第一步滤波
    //Utils::MeanBlur(out_hist1_x, out_hist_x, 3); // 第二步滤波
    //Utils::MeanBlur(out_hist_x, out_hist1_x, 3); // 第三步滤波

    //Utils::drawHistogramFliter(out_hist1_x, "MMM"); // 绘制1次中值2次均值滤波后的直方图

    // y轴直方图
    vector<int> out_hist_y(255,0); // 存储直方图数据的数组，初始值为0
    Utils::CalcHist(x_values, y_values, out_hist_y, 0); // 绘制y的直方图

    vector<int> out_hist1_y(255,0); // 存储滤波直方图数据的数组，初始值为0
    Utils::MedianBlur(out_hist_y, out_hist1_y, 3); //第一步滤波
    Utils::MeanBlur(out_hist1_y, out_hist_y, 3); // 第二步滤波
    Utils::MeanBlur(out_hist_y, out_hist1_y, 3); // 第三步滤波
    //Utils::drawHistogramFliter(out_hist1_y, "MMM"); // 绘制1次中值2次均值滤波后的直方图


    // 散点图旋转
    // 旋转角度
    double angle = 45;

    // x轴旋转直方图
    vector<double> in_hist_x_rotated = Utils::rotatePointsX(x_values, y_values, angle);
    // Utils::CalcHist(x_rotated_values, y_rotated_values, out_hist_x_rotated, 1); // 绘制x的直方图

    // y轴旋转直方图
    vector<double> in_hist_y_rotated = Utils::rotatePointsY(x_values, y_values, angle);
    vector<int> out_hist_y_rotated;
    Utils::CalcHist2(in_hist_x_rotated, in_hist_y_rotated, out_hist_y_rotated, 0); // 绘制x的直方图



    waitKey(0);

    return 0;
}
