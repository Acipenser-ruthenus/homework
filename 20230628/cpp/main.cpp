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
    // ��ȡCSV�ļ�
    std::ifstream file("C:/Users/U/Desktop/1(1).csv");
    if (!file.is_open())
    {
        std::cout << "Failed to open CSV file." << std::endl;
        return -1;
    }

    std::vector<DataPoint> data;

    std::string line;
    // ����������
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;

        DataPoint point;

        // ��ȡÿ�е�����
        std::getline(ss, token, ',');
        point.x = std::stoi(token);

        std::getline(ss, token, ',');
        point.y = std::stoi(token);

        std::getline(ss, token, ',');
        point.type = std::stoi(token);

        data.push_back(point);
    }

    file.close();

    // ����һ����ɫ������ͼ��
    //cv::Mat image(500, 500,CV_8UC3, cv::Scalar(255, 255, 255));
    // ����һ�����б߾��ͼ��
    int margin = 30; // ���ñ߾��С
    cv::Mat image(250 + 2 * margin, 250 + 2 * margin, CV_8UC3, cv::Scalar(255, 255, 255));

    // �������ɢ���ƫ����
    int xOffset = margin;
    int yOffset = margin;



    // �������͵���ɫ��ӳ��
    std::unordered_map<int, cv::Scalar> colorMap;
    colorMap[1] = cv::Scalar(0, 0, 255);    // ����1Ϊ��ɫ
    colorMap[2] = cv::Scalar(0, 255, 0);    // ����2Ϊ��ɫ
    colorMap[3] = cv::Scalar(255, 0, 0);    // ����3Ϊ��ɫ
    colorMap[0] = cv::Scalar(0, 255, 255);  // ����4Ϊ��ɫ
    colorMap[5] = cv::Scalar(255, 0, 255);  // ����5Ϊ���ɫ
    colorMap[6] = cv::Scalar(255, 255, 0);  // ����6Ϊ��ɫ
    colorMap[9] = cv::Scalar(128, 128, 128);// ����7Ϊ��ɫ
    colorMap[14] = cv::Scalar(0, 128, 128);  // ����8Ϊ����ɫ


    // ��ͼ���ϻ���ɢ��
    for (const auto& point : data)
    {
        int x = point.x + xOffset;
        int y = image.rows - (point.y + yOffset);

        cv::Scalar color = colorMap[point.type];

        cv::circle(image, cv::Point(x, y), 2, color, -1); // ��ͼ���ϻ���ʵ��Բ
    }

    // ���������ͱ�ǩ
    int axisPadding = 30; // ��ı߾�
    int axisLength = 400; // ��ĳ���

    // ����x��
    cv::line(image, cv::Point(axisPadding, image.rows - axisPadding), cv::Point(image.cols - axisPadding, image.rows - axisPadding), cv::Scalar(0, 0, 0), 2);
    cv::putText(image, "X", cv::Point(image.cols - axisPadding, image.rows - axisPadding + 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);

    // ����y��
    cv::line(image, cv::Point(axisPadding, image.rows - axisPadding), cv::Point(axisPadding, axisPadding), cv::Scalar(0, 0, 0), 2);
    cv::putText(image, "Y", cv::Point(axisPadding - 20, axisPadding), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);

    // ��ʾͼ��
    cv::imshow("Scatter Plot", image);
    cv::waitKey(0);


    return 0;
}
