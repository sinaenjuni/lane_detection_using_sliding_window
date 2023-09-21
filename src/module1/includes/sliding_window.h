#include "preprocessing.h"
#include <iostream>
#include <opencv2/opencv.hpp>

enum class PointType{
    LEFT,
    RIGHT
};

class SlidingWindow{
    public:
        SlidingWindow(double width, double height, double num_layers, double win_width, double win_height);
        ~SlidingWindow();
        void calc_rows_sum(preprocessing::umat_t src);
        void get_init_point(PointType type);
        void get_point(size_t index, std::vector<cv::Point> &points, std::vector<cv::Point> &prev_points);
        void get_windows(preprocessing::umat_t src);
        void draw_box(preprocessing::umat_t &src);
        void draw_poly(preprocessing::umat_t &src);
    private:
        double WIDTH_, HEIGHT_;
        size_t init_layer_ind_;
        double num_layers_, win_height_, win_width_, img_center_;
        preprocessing::umat_t window_;

        std::vector<preprocessing::umat_t> rows_sum_;
        std::vector<cv::Point> l_points_;
        std::vector<cv::Point> l_prev_points_;
        std::vector<cv::Point> r_points_;
        std::vector<cv::Point> r_prev_points_;
        std::vector<cv::Point> poly_points_;

};