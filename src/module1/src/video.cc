#include "video.h"
#include "sliding_window.h"

VideoManager::VideoManager(std::string path) : path_(path) {
    std::cout<< path_ << "\n";
    cap_.open(path_);
    FPS_ = cap_.get(cv::CAP_PROP_FPS);
    WIDTH_ = cap_.get(cv::CAP_PROP_FRAME_WIDTH);
    HEIGHT_ = cap_.get(cv::CAP_PROP_FRAME_HEIGHT);

    src_pts_.push_back(cv::Point2f((WIDTH_/2) - (WIDTH_*0.05), 480));
    src_pts_.push_back(cv::Point2f((WIDTH_/2) + (WIDTH_*0.05), 480)); 
    src_pts_.push_back(cv::Point2f((WIDTH_/2) + (WIDTH_*0.3), 690)); 
    src_pts_.push_back(cv::Point2f((WIDTH_/2) - (WIDTH_*0.3), 690));

    offset_ = WIDTH_ * 0.25;
	dst_pts_.push_back(cv::Point2f((WIDTH_/2) - offset_, 0)); 
    dst_pts_.push_back(cv::Point2f((WIDTH_/2) + offset_, 0)); 
    dst_pts_.push_back(cv::Point2f((WIDTH_/2) + offset_, HEIGHT_)); 
    dst_pts_.push_back(cv::Point2f((WIDTH_/2) - offset_, HEIGHT_));

    perM_ = cv::getPerspectiveTransform(src_pts_, dst_pts_);
    inv_perM_ = cv::getPerspectiveTransform(dst_pts_, src_pts_);

    win_height_ = 72;
    win_width_ = 120;
    num_layers_ = ceil(HEIGHT_ / win_height_);
    
}


void normalize_HLS_L(img_t src, img_t &det, int lowThres = 200){
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
void normalize_LAB_B(img_t src, img_t &det, int lowThres = 180){
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


void VideoManager::run(){
    cv::namedWindow("image");
    cv::setMouseCallback("image", on_moues);

    SlidingWindow sd(WIDTH_, HEIGHT_, num_layers_, win_height_, win_width_);

    while(true){
        tick_meter_.start();
        
        img_t frame;
        cap_ >> frame;
        if (frame.empty()) break;

        img_t roi;
        cv::warpPerspective(frame, roi, perM_, cv::Size(WIDTH_, HEIGHT_), cv::INTER_LINEAR);

        double m = 128 - cv::mean(roi)[0];
        cv::add(roi, m, roi);

        img_t hls, lab, gray;
        normalize_HLS_L(roi, hls, 210);
        normalize_LAB_B(roi, lab, 200);
        cv::bitwise_or(hls, lab, gray);

        sd.get_windows(gray);


        // std::vector<img_t> sums(num_layers_);
        // for(int i=0; i<num_layers_; i++){
            // cv::Rect box(0, i*win_height_, WIDTH_, win_height_);

            // img_t sum;
            // cv::reduce(gray(box), sums[i], 0, cv::REDUCE_SUM, CV_32F);
            // img_t signal;
            // cv::filter2D(sum, sums[i], -1, window);

            // double max_val;
            // cv::Point max_loc;

            // cv::minMaxLoc(signal, nullptr, &max_val, nullptr, &max_loc);
            // std::cout << max_loc.x << " " << max_loc.y << " " << max_val <<"\n"; 
            
            // cv::rectangle(gray, box, cv::Scalar(255,0,0), 2, cv::LINE_AA);
            // cv::rectangle(gray, cv::Rect(max_loc.x-(win_width_/2), i*win_height_, win_width_, win_height_), cv::Scalar(255,0,0), 2, cv::LINE_AA);
            // std::cout << i << " (" << i*win_height_ << ", " << i*win_height_+win_height_ << ") " << sum << " ";
        // }

        // cv::Point init_point;
        // sd.get_init_point(sums[num_layers_-1], init_point, PointType::LEFT);
        // sd.draw_box(gray, num_layers_-1, init_point);
        // std::cout << init_point.x << " ";
        // // std::cout << static_cast<int>(WIDTH_/2) << " ";
        // sd.get_init_point(sums[num_layers_-1], init_point, PointType::RIGHT);
        // // init_point.x = init_point.x + WIDTH_/2;
        // sd.draw_box(gray, num_layers_-1, init_point);
        // std::cout << init_point.x << "\n";

        // // cv::Point init_point;
        // sd.get_point(sums[num_layers_-2], init_point, init_point.x-25, init_point.x+25);
        // sd.draw_box(gray, num_layers_-2, init_point);


        // for(for i=0;){
        //     img_t signal;
        //     cv::filter2D(sum, signal, -1, window);
        //     double max_val;
        //     cv::Point max_loc;

        //     cv::minMaxLoc(signal, nullptr, &max_val, nullptr, &max_loc);
        //     // std::cout << max_loc.x << " " << max_loc.y << " " << max_val <<"\n"; 
            
        //     // cv::rectangle(gray, box, cv::Scalar(255,0,0), 2, cv::LINE_AA);
        //     cv::rectangle(gray, cv::Rect(max_loc.x-(win_width_/2), i*win_height_, win_width_, win_height_), cv::Scalar(255,0,0), 2, cv::LINE_AA);
        //     // std::cout << i << " (" << i*win_height_ << ", " << i*win_height_+win_height_ << ") " << sum << " ";
        // }
        // std::cout << sums[0] << " ";
        // std::cout<<"\n";


        // img_t box;
        // box = src(cv::Rect(0, num_layer *  window_height, WIDTH/2, window_height));
        // img_t sum;
        // cv::reduce(roi, sum, 0, cv::REDUCE_SUM, CV_32F);
        // std::cout << sum.size << "\n";
        // cv::imshow("sum", sum);

        


        // color_threshold(frame, gray, {50,255}, {50,255});
        // cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        // cv::cvtColor(frame, gray, cv::COLOR_BGR2HSV);
        // cv::extractChannel(gray, gray, 2);
        // cv::blur(gray, gray, cv::Size(3, 3));

        // cv::subtract(255, gray, gray);


        

        // img_t bin;
        // cv::inRange(gray, 0, 100, bin);


        // img_t sobel;
        // cv::Sobel(roi, sobel, CV_32F, 1, 0, 3, 1, 5);
        // img_t edge;
        // cv::convertScaleAbs(sobel, edge);
        // // get_canny_edge(gray, edge);

        // img_t andbit;
        // cv::bitwise_or(bin, edge, andbit);



 
        // // cv::GaussianBlur(hsv, hsv, cv::Size(5, 5), 1);
        // cv::blur(hsv, hsv, cv::Size(10, 10));q
        // double m = 128 - cv::mean(hsv)[0];
        // cv::subtract(hsv, m, hsv);

        // img_t edge;
        // cv::Canny(hsv, edge, 0, 10);

        // img_t abs_sobel_x, abs_sobel_y;
        // cv::Sobel(roi, abs_sobel_x, CV_32F, 1, 0, 3);
        // cv::Sobel(roi, abs_sobel_y, CV_32F, 0, 1, 3);
        // img_t scaled_x, scaled_y;
        // cv::convertScaleAbs(abs_sobel_x, scaled_x);
        // cv::convertScaleAbs(abs_sobel_y, scaled_y);

        // img_t edge;
        // cv::threshold(scaled_x, scaled_x, 20, 255, cv::THRESH_BINARY);
        // cv::threshold(scaled_y, scaled_y, 40, 255, cv::THRESH_BINARY);
        // cv::bitwise_or(scaled_x, scaled_y, edge);

        // img_t binhsv;
        // cv::inRange(hsv, 170, 200, binhsv);


        // img_t hls;
        // cv::cvtColor(roi, hls, cv::COLOR_BGR2HLS);
        // cv::extractChannel(hls, hls, 1);
        // // cv::warpPerspective(hls, hls, perM_, cv::Size(WIDTH_, HEIGHT_), cv::INTER_LINEAR);

        // // // cv::GaussianBlur(hls, hls, cv::Size(5, 5), 1);
        // cv::blur(hls, hls, cv::Size(7, 7));
        // double m = 128 - cv::mean(hls)[0];
        // cv::subtract(m, hls, hls);
        
        // img_t binhls;
        // cv::inRange(hls, 250, 255, binhls);
        
        // double m = 128 - cv::mean(hls)[0];
        // // cv::subtract(255, hls, hls);
        // cv::subtract(255-m, hls, hls);
        // std::cout << cv::mean(hls)[0] << "\n";

        // img_t bin;
        // cv::inRange(hls, 0, 70, bin);


        // img_t gray;
        // cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // cv::cvtColor(frame, gray, cv::COLOR_BGR2HSV);
        // cv::extractChannel(gray, gray, 2);

        // cv::cvtColor(frame, gray, cv::COLOR_BGR2HLS);
        // cv::extractChannel(gray, gray, 1);
        
        // cv::blur(gray, gray, cv::Size(7, 7));
        // cv::subtract(255, gray, gray);


        // img_t abs_sobel_x, abs_sobel_y;
        // cv::Sobel(gray, abs_sobel_x, CV_32F, 1, 0, 3);
        // cv::Sobel(gray, abs_sobel_y, CV_32F, 0, 1, 3);

        // img_t scaled_x, scaled_y;
        // cv::convertScaleAbs(abs_sobel_x, scaled_x);
        // cv::convertScaleAbs(abs_sobel_y, scaled_y);

        // img_t edge;
        // cv::threshold(scaled_x, scaled_x, 20, 255, cv::THRESH_BINARY);
        // cv::threshold(scaled_y, scaled_y, 40, 255, cv::THRESH_BINARY);
        // cv::bitwise_and(scaled_x, scaled_y, edge);
        // cv::bitwise_or(bin, edge, edge);

        // cv::warpPerspective(edge, warped, M, cv::Size(WIDTH, HEIGHT), cv::INTER_LINEAR);
        // sliding_window(warped);
        // std::vector<img_t> images = {gray, scaled_x, scaled_y, edge, warped};
        // cv::hconcat(images, ret);
        // cv::imshow("Window", ret);


        // std::cout << frame.size << "\n";
        // for (auto i : src_pts_) {
        //     cv::circle(frame, i, 10, cv::Scalar(255, 0, 0), -1);
        // }
        // for (auto i : dst_pts_) {
        //     cv::circle(frame, i, 10, cv::Scalar(0, 0, 255), -1);
        // }


        // cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        // cv::GaussianBlur(frame, frame, cv::Size(3, 3), 1);
        // cv::rectangle(frame, roi, cv::Scalar(0,0,255));

        // cv::imshow("image", frame);
        img_t ret;
        // cv::cvtColor(hsv, hsv, cv::COLOR_GRAY2BGR);
        // cv::cvtColor(binhsv, binhsv, cv::COLOR_GRAY2BGR);

        // cv::cvtColor(scaled_x, scaled_x, cv::COLOR_GRAY2BGR);
        // cv::cvtColor(roi, roi, cv::COLOR_GRAY2BGR);
        cv::cvtColor(gray, gray, cv::COLOR_GRAY2BGR);
        // cv::cvtColor(hls, hls, cv::COLOR_GRAY2BGR);
        // cv::cvtColor(lab, lab, cv::COLOR_GRAY2BGR);
        // cv::cvtColor(andbit, andbit, cv::COLOR_GRAY2BGR);

        std::vector<img_t> images;
        images.push_back(frame);
        // images.push_back(lab);
        // images.push_back(hls);
        images.push_back(gray);
        // images.push_back(roi);
        // images.push_back(edge);
        // images.push_back(bin);
        // images.push_back(andbit);
        cv::hconcat(images, ret);
        cv::imshow("image", ret);

        tick_meter_.stop();
        // std::cout << 1./tick_meter_.getTimeSec() << "\n";
        tick_meter_.reset();
        int key = cv::waitKey(1);
        if (key == 'q'){
            break;
        }
    }

}

VideoManager::~VideoManager(){
    cap_.release();
    cv::destroyAllWindows();
    std::cout<<"Call distroyer\n";
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



