#include <opencv2/opencv.hpp>
#include <fstream>
#include <sstream>

struct DataPoint
{
    int x;
    int y;
    int type;
};

int main()
{
    // 读取CSV文件
    std::ifstream file("C:/Users/U/Desktop/1(1).csv");
    if (!file.is_open())
    {
        std::cout << "Failed to open CSV file." << std::endl;
        return -1;
    }

    std::vector<DataPoint> data;

    std::string line;
    // 跳过标题行
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;

        DataPoint point;

        // 提取每行的数据
        std::getline(ss, token, ',');
        point.x = std::stoi(token);

        std::getline(ss, token, ',');
        point.y = std::stoi(token);

        std::getline(ss, token, ',');
        point.type = std::stoi(token);

        data.push_back(point);
    }

    file.close();

    // 创建一个白色背景的图像
    //cv::Mat image(500, 500,CV_8UC3, cv::Scalar(255, 255, 255));
    // 创建一个具有边距的图像
    int margin = 30; // 设置边距大小
    cv::Mat image(250 + 2 * margin, 250 + 2 * margin, CV_8UC3, cv::Scalar(255, 255, 255));

    // 计算绘制散点的偏移量
    int xOffset = margin;
    int yOffset = margin;



    // 设置类型到颜色的映射
    std::unordered_map<int, cv::Scalar> colorMap;
    colorMap[1] = cv::Scalar(0, 0, 255);    // 类型1为红色
    colorMap[2] = cv::Scalar(0, 255, 0);    // 类型2为绿色
    colorMap[3] = cv::Scalar(255, 0, 0);    // 类型3为蓝色
    colorMap[0] = cv::Scalar(0, 255, 255);  // 类型4为青色
    colorMap[5] = cv::Scalar(255, 0, 255);  // 类型5为洋红色
    colorMap[6] = cv::Scalar(255, 255, 0);  // 类型6为黄色
    colorMap[9] = cv::Scalar(128, 128, 128);// 类型7为灰色
    colorMap[14] = cv::Scalar(0, 128, 128);  // 类型8为深青色


    // 在图像上绘制散点
    for (const auto& point : data)
    {
        int x = point.x + xOffset;
        int y = image.rows - (point.y + yOffset);

        cv::Scalar color = colorMap[point.type];

        cv::circle(image, cv::Point(x, y), 2, color, -1); // 在图像上绘制实心圆
    }

    // 添加坐标轴和标签
    int axisPadding = 30; // 轴的边距
    int axisLength = 400; // 轴的长度

    // 绘制x轴
    cv::line(image, cv::Point(axisPadding, image.rows - axisPadding), cv::Point(image.cols - axisPadding, image.rows - axisPadding), cv::Scalar(0, 0, 0), 2);
    cv::putText(image, "X", cv::Point(image.cols - axisPadding, image.rows - axisPadding + 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);

    // 绘制y轴
    cv::line(image, cv::Point(axisPadding, image.rows - axisPadding), cv::Point(axisPadding, axisPadding), cv::Scalar(0, 0, 0), 2);
    cv::putText(image, "Y", cv::Point(axisPadding - 20, axisPadding), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);

    // 显示图像
    cv::imshow("Scatter Plot", image);
    cv::waitKey(0);


    return 0;
}
