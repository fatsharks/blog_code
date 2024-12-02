#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>
using namespace cv;
constexpr char* img_path = "/home/kyojinliu/code/sth/src/visual/4.2.05.tiff";

void fast_corner_extract_test(cv::Mat &img)
{

    std::vector<cv::KeyPoint> key_points;
    auto fast = cv::FastFeatureDetector::create(50);
    fast->detect(img, key_points);

    cv::drawKeypoints(img, key_points, img, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG);

    cv::imshow("image with kp", img);
    cvWaitKey(0);

}

void harris_corner_extract_test(cv::Mat &img)
{
    std::vector<cv::KeyPoint> key_points;

    cv::Mat img_gray;

    cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
}

int v_slam_entr(void)
{
    cv::Mat origin_img = cv::imread(img_path);
    fast_corner_extract_test(origin_img);
    return 0;
}