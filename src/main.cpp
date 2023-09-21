#include "preprocessing.h"
#include "sliding_window.h"
#include "thread_pool.h"
#include <easy/profiler.h>

int main(int, char**){
    EASY_PROFILER_ENABLE;
    // profiler::startListen();

    const std::string PATH = "../data/road_video1.mp4";
    cv::VideoCapture cap(PATH);
    cv::namedWindow("view");

    const double fps = cap.get(cv::CAP_PROP_FPS);
    const double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    const double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::vector<cv::Point2f> src_pts;
    src_pts.push_back(cv::Point2f((width/2) - (width*0.05), 480));
    src_pts.push_back(cv::Point2f((width/2) + (width*0.05), 480)); 
    src_pts.push_back(cv::Point2f((width/2) + (width*0.3), 690)); 
    src_pts.push_back(cv::Point2f((width/2) - (width*0.3), 690));

    const double offset = width * 0.25;
    std::vector<cv::Point2f> dst_pts;
	dst_pts.push_back(cv::Point2f((width/2) - offset, 0)); 
    dst_pts.push_back(cv::Point2f((width/2) + offset, 0)); 
    dst_pts.push_back(cv::Point2f((width/2) + offset, height)); 
    dst_pts.push_back(cv::Point2f((width/2) - offset, height));

    const preprocessing::mat_t perM = cv::getPerspectiveTransform(src_pts, dst_pts);
    const preprocessing::mat_t inv_perM = cv::getPerspectiveTransform(dst_pts, src_pts);

    const double win_height = 72;
    const double win_width = 120;
    const double num_layers = ceil(height / win_height);

    SlidingWindow sliding_window(width, height, num_layers, win_width, win_height);

    // cv::TickMeter tick_meter;
    
    // 영상 저장에 관한 코드
	// cv::VideoWriter vw;
    // vw.open("../data/output_video.mp4", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 
    //                     fps, cv::Size(width, height), true);

	// if (!vw.isOpened())
	// {
	// 	std::cout << "Can't write video !!! check setting" << std::endl;
	// 	return -1;
	// }

    // ThreadPool pool(1);


    while(true){
        EASY_BLOCK("main_loop");
        preprocessing::umat_t frame;
        // tick_meter.start();
        cap >> frame;
        if (frame.empty()) break;
        // std::cout << cap.get(cv::CAP_PROP_POS_FRAMES) << "\n";
        if (cap.get(cv::CAP_PROP_POS_FRAMES) == 40) break;

        // cv::imshow("frame", frame);

        preprocessing::umat_t roi;
        preprocessing::apply_perspective_transform(frame, roi, perM, width, height);
        // cv::imshow("roi", roi);

        preprocessing::umat_t bin;
        preprocessing::apply_color_threshold(roi, bin);
        // cv::imshow("bin", bin);
        // preprocessing::umat_t hls, lab;
        // pool.EnqueueJob([&roi, &hls]{preprocessing::normalize_HLS_L(roi, hls, 210);});
        // pool.EnqueueJob([&roi, &lab]{preprocessing::normalize_LAB_B(roi, lab, 200);});

        // cv::bitwise_or(hls, lab, bin);
        // cv::imshow("bin", bin);

        sliding_window.get_windows(bin);
        // preprocessing::umat_t sdbox(height, width, CV_8UC3);
        // sliding_window.draw_box(sdbox);
        // cv::imshow("sdbox", sdbox);


        preprocessing::umat_t poly(height, width, CV_8UC3);
        sliding_window.draw_poly(poly);
        // // cv::imshow("poly", poly);

        preprocessing::umat_t weight;
        preprocessing::apply_perspective_transform(poly, weight, inv_perM, width, height);
        
        // // tick_meter.stop();
        // // std::cout << 1./tick_meter.getTimeSec() << "\n";
        // // std::cout << tick_meter.getTimeSec() << "\n";
        // // tick_meter.reset();

        // // tick_meter.stop();
        // // std::cout << tick_meter.getAvgTimeMilli() << "\n";

        // // cv::cvtColor(box, box, cv::COLOR_GRAY2BGR);
        // // std::vector<preprocessing::umat_t> images;
        // // images.push_back(frame);
        // // images.push_back(weight);
        // // preprocessing::umat_t ret;
        // // cv::hconcat(images, ret);
        // // cv::imshow("veiw", ret);

        EASY_BLOCK("addWeight");
        cv::addWeighted(frame, 1, weight, 1, 0, frame);
        EASY_END_BLOCK;

        // EASY_BLOCK("imshow")
        // cv::imshow("veiw", frame);
        // EASY_END_BLOCK;
        // // // vw << frame;

        // int key = cv::waitKey(1);
        // if (key == 'q'){
        //     break;
        // }
        EASY_END_BLOCK;
    }


    cap.release();
    cv::destroyAllWindows();
    profiler::dumpBlocksToFile("./test_profile.prof");

}
