#include <iostream>
#include <opencv2/opencv.hpp>


namespace preprocessing
{

// typedef cv::UMat img_t;
typedef cv::UMat umat_t;
typedef cv::Mat mat_t;


void on_moues(int event, int x, int y, int flags, void*);
double_t get_median(cv::Mat image);
double_t get_median(umat_t image);
void get_canny_edge(cv::Mat src, cv::Mat dst, float32_t sigma);
void get_canny_edge(umat_t src, umat_t dst, float32_t sigma);
void normalize_HLS_L(const umat_t &src, umat_t &det, int lowThres);
void normalize_LAB_B(const umat_t &src, umat_t &det, int lowThres);
void apply_color_threshold(const umat_t &src, umat_t &dst);
void apply_perspective_transform(const umat_t &src, umat_t &dst, const mat_t &perM, const double &width, const double &height);
} // namespace preprocessing