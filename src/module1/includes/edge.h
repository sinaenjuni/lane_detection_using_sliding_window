


double_t get_median(cv::Mat src){
    std::vector<double_t> src_vec;
    src = src.reshape(0,1);
    src.copyTo(src_vec, cv::noArray());
    std::nth_element(src_vec.begin(), src_vec.begin() + src_vec.size() / 2, src_vec.end());
    return src_vec[src_vec.size() / 2];
}
double_t get_median(cv::UMat src){
    std::vector<double_t> src_vec;
    src = src.reshape(0,1);
    src.copyTo(src_vec, cv::noArray());
    std::nth_element(src_vec.begin(), src_vec.begin() + src_vec.size() / 2, src_vec.end());
    return src_vec[src_vec.size() / 2];
}
void get_canny_edge(cv::Mat src, cv::Mat &det, float32_t sigma=0.33){
    double_t median = get_median(src);
    int32_t lower;
    int32_t upper;

    lower = (int)std::max(0.0  , (1.0-sigma) * median);
    upper = (int)std::min(255.0, (1.0+sigma) * median);
    cv::Canny(src, det, lower, upper);
}
void get_canny_edge(cv::UMat src, cv::UMat &det, float32_t sigma=0.33){
    double_t median = get_median(src);
    int32_t lower;
    int32_t upper;

    lower = (int)std::max(0.0  , (1.0-sigma) * median);
    upper = (int)std::min(255.0, (1.0+sigma) * median);
    cv::Canny(src, det, lower, upper);
}