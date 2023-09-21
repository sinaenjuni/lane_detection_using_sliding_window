#include <iostream>
#include <Eigen/Dense>
// #include <Eigen/Core>
#include "preprocessing.h"

namespace fit_poly{
Eigen::VectorXd polyfit(Eigen::VectorXd xvals, Eigen::VectorXd yvals, int degree);
Eigen::VectorXd polyfit(std::vector<cv::Point>, int degree);
double polyeval(Eigen::VectorXd coeffs, double x);
}