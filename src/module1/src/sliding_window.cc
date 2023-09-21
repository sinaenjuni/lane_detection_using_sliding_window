#include "preprocessing.h"
#include "sliding_window.h"
#include "fit_poly.h"
#include <easy/profiler.h>


SlidingWindow::SlidingWindow(double width, double height, double num_layers, double win_width, double win_height){
    WIDTH_ = width;
    HEIGHT_ = height;
    num_layers_ = num_layers;
    win_height_ = win_height;
    win_width_ = win_width;
    img_center_ = WIDTH_/2;
    init_layer_ind_ = num_layers_-1;
    window_ = preprocessing::umat_t::ones(win_width_, 1, CV_8U);

    rows_sum_ = std::vector<preprocessing::umat_t>(num_layers_);
    l_points_ = std::vector<cv::Point>(num_layers_);
    l_prev_points_ = std::vector<cv::Point>(num_layers_);
    r_points_ = std::vector<cv::Point>(num_layers_);
    r_prev_points_ = std::vector<cv::Point>(num_layers_);

}

SlidingWindow::~SlidingWindow(){
}

void SlidingWindow::calc_rows_sum(preprocessing::umat_t src){
    EASY_FUNCTION(profiler::colors::Red200); 

    for(size_t i=0; i<num_layers_; i++){
        cv::Rect box(0, i*win_height_, WIDTH_, win_height_);
        cv::reduce(src(box), rows_sum_[i], 0, cv::REDUCE_SUM, CV_32F);
    }
}

void SlidingWindow::get_init_point(PointType type){
    EASY_FUNCTION(profiler::colors::Red200); 
    
    preprocessing::umat_t sum_temp;
    if(type == PointType::LEFT){
        cv::filter2D(rows_sum_[init_layer_ind_](cv::Range(0, 1), cv::Range(0, img_center_)), sum_temp, -1, window_);
        cv::minMaxLoc(sum_temp, nullptr, nullptr, nullptr, &l_points_[init_layer_ind_]);
        l_points_[init_layer_ind_].y = HEIGHT_ - win_height_/2;

        if((l_prev_points_[init_layer_ind_].x != 0) && (abs(l_points_[init_layer_ind_].x - l_prev_points_[init_layer_ind_].x) > 100)) {
            l_points_[init_layer_ind_].x = l_prev_points_[init_layer_ind_].x;
        }else{
            l_prev_points_[init_layer_ind_] = l_points_[init_layer_ind_];
        }
    }else{
        cv::filter2D(rows_sum_[init_layer_ind_](cv::Range(0, 1), cv::Range(img_center_, WIDTH_)), sum_temp, -1, window_);
        cv::minMaxLoc(sum_temp, nullptr, nullptr, nullptr, &r_points_[init_layer_ind_]);
        r_points_[init_layer_ind_].x = r_points_[init_layer_ind_].x+img_center_;
        r_points_[init_layer_ind_].y = HEIGHT_ - win_height_/2;

        if((r_prev_points_[init_layer_ind_].x != 0) && (r_prev_points_[init_layer_ind_].x != img_center_) && 
         (abs(r_points_[init_layer_ind_].x - r_prev_points_[init_layer_ind_].x) > 100)) {
            r_points_[init_layer_ind_].x = r_prev_points_[init_layer_ind_].x;
        }else{
            r_prev_points_[init_layer_ind_] = r_points_[init_layer_ind_];
        }
    }
}

void SlidingWindow::get_point(size_t index, std::vector<cv::Point> &points, std::vector<cv::Point> &prev_points){
    EASY_FUNCTION(profiler::colors::Red200); 

    // std::cout << index << "::" << points[index+1].x <<  " ::min_range:: " << points[index].x << "\n";
    size_t min_range = points[index+1].x - win_width_/2;
    size_t max_range = points[index+1].x + win_width_/2;
    // if(min_range < 0){return;}
    // if(max_range > WIDTH_) {return;}

    preprocessing::umat_t temp_sum;
    cv::filter2D(rows_sum_[index](cv::Range(0, 1), cv::Range(min_range, max_range)), temp_sum, -1, window_);

    cv::Point max_loc;
    cv::minMaxLoc(temp_sum, nullptr, nullptr, nullptr, &max_loc);
    if(max_loc.x == 0){
        return;
    } 
    // max_loc.x = points[index+1].x;
    max_loc.y = (index*win_height_) + win_height_/2;
    max_loc.x += min_range;
    points[index] = max_loc;
    
    // if (max_loc.x == 0){
    // }else{
    //     max_loc.x += min_range;
    // }

    // if (max_loc.x == min_range){
    // }else{
    //     points[index] = max_loc;
    //     prev_points[index] = max_loc;
    // }

}


void SlidingWindow::get_windows(preprocessing::umat_t src){
    EASY_FUNCTION(profiler::colors::Blue300); 

    calc_rows_sum(src);

    get_init_point(PointType::LEFT);
    get_init_point(PointType::RIGHT);

    for(int i=num_layers_-2; i>=0; i--){
        get_point(i, l_points_, l_prev_points_);
        get_point(i, r_points_, r_prev_points_);
    }

    auto l_coeff = fit_poly::polyfit(l_points_, 2); // 2차식 (0:상수, 1:x, 2:x^2)
    auto r_coeff = fit_poly::polyfit(r_points_, 2); // 2차식 (0:상수, 1:x, 2:x^2)

    for (int i=0; i<=700; i+=100) {
        poly_points_.push_back(cv::Point(fit_poly::polyeval(l_coeff, i), i));
    }

    for (int i=700; i>=0; i-=100) {
        poly_points_.push_back(cv::Point(fit_poly::polyeval(r_coeff, i), i));
    }
}

void SlidingWindow::draw_box(preprocessing::umat_t &src){
    for(int i=0; i<num_layers_; i++){
        if(l_points_[i].x != 0){
            cv::rectangle(src, cv::Rect(l_points_[i].x-(win_width_/2), i * win_height_, win_width_, win_height_), cv::Scalar(255,0,0), 2, cv::LINE_AA);
            cv::circle(src, l_points_[i], 20, cv::Scalar(0,0,255), -1, cv::LINE_AA);
        }

        if(r_points_[i].x != 0){
            cv::rectangle(src, cv::Rect(r_points_[i].x-(win_width_/2), i * win_height_, win_width_, win_height_), cv::Scalar(255,0,0), 2, cv::LINE_AA);
            cv::circle(src, r_points_[i], 20, cv::Scalar(0,0,255), -1, cv::LINE_AA);
        }
    }
}

void SlidingWindow::draw_poly(preprocessing::umat_t &src){
    EASY_FUNCTION(profiler::colors::Red200); 

    cv::fillPoly(src, poly_points_, cv::Scalar(0,255,0), cv::LINE_AA);
    // cv::polylines(src, poly_points_, true, cv::Scalar(0,255,0), 10, cv::LINE_AA);
    poly_points_.clear();
}

