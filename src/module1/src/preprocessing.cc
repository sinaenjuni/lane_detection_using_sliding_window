#include "preprocessing.h"
#include <easy/profiler.h>
#include <thread>
#include <future>




namespace preprocessing{
void apply_perspective_transform(const umat_t &src, umat_t &dst, const mat_t &perM, const double &width, const double &height){
    EASY_FUNCTION(profiler::colors::Blue100); 
    cv::warpPerspective(src, dst, perM, cv::Size(width, height), cv::INTER_LINEAR);
}

void normalize_HLS_L(const umat_t &src, umat_t &det, int lowThres = 200){
        EASY_FUNCTION(profiler::colors::Red200); 

        double maxVal;
        // get a single channel img(filtered one.)
        cv::cvtColor(src, det, cv::COLOR_BGR2HLS);
        cv::extractChannel(det, det, 1);
        // get max, min value of the matrix.
        cv::minMaxLoc(det, nullptr, &maxVal, nullptr, nullptr);
        cv::multiply((255 / maxVal), det, det);
        // apply threshold for L channel.
        cv::threshold(det, det, lowThres, 255, cv::THRESH_BINARY);
}

void normalize_LAB_B(const umat_t &src, umat_t &det, int lowThres = 180){
        EASY_FUNCTION(profiler::colors::Red200); 

        double maxVal;
        // get a single channel img(filtered one.)
        cv::cvtColor(src, det, cv::COLOR_BGR2Lab);
        cv::extractChannel(det, det, 2);
        // get max, min value of the matrix.
        cv::minMaxLoc(det, nullptr, &maxVal, nullptr, nullptr);
        cv::multiply((255 / maxVal), det, det);
        // apply threshold for L channel.
        cv::threshold(det, det, lowThres, 255, cv::THRESH_BINARY);
}

void apply_color_threshold(const umat_t &src, umat_t &dst){
    EASY_FUNCTION(profiler::colors::Blue200); 

    umat_t hls, lab;
    normalize_HLS_L(src, hls, 210);
    normalize_LAB_B(src, lab, 200);
    // std::thread _t1([&]{ normalize_HLS_L(src, hls, 210); });
    // std::thread _t2([&]{ normalize_LAB_B(src, lab, 200); });
    
    // _t1.join();
    // _t2.join();

    // auto f1 = std::async([&]() { normalize_HLS_L(src, hls, 210); });
    // auto f2 = std::async([&]() { normalize_LAB_B(src, lab, 200); });
 
    // f1.get();
    // f2.get();

    cv::bitwise_or(hls, lab, dst);
}

void on_moues(int event, int x, int y, int flags, void*){
    switch (event) {
	case cv::EVENT_LBUTTONDOWN:
		// ptOld = Point(x, y);
		std::cout << "EVENT_LBUTTONDOWN: " << x << ", " << y << "\n";
        break;
    default:
        break;
    }
}

}
