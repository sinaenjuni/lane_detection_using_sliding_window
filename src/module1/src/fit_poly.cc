#include "fit_poly.h"
#include <easy/profiler.h>

namespace fit_poly
{
Eigen::VectorXd polyfit(Eigen::VectorXd xvals, Eigen::VectorXd yvals, int degree) 
{       
    assert(xvals.size() == yvals.size());
    assert(degree >= 1 && degree <= xvals.size() - 1);
    size_t N = xvals.size();

    Eigen::MatrixXd A(N, degree + 1);
    A.col(0) = Eigen::VectorXd::Ones(N);

    for (Eigen::Index i = 0; i < degree; ++i)
        A.col(i + 1) = A.col(i).cwiseProduct(xvals);

    // for (int i = 0; i < xvals.size(); i++) {
    //     A(i, 0) = 1.0;
    // }

    // for (int j = 0; j < xvals.size(); j++) {
    //     for (int i = 0; i < degree; i++) {
    //         A(j, i + 1) = A(j, i) * xvals(j);
    //     }
    // }

    // std::cout << A << "\n";

    auto Q = A.householderQr();
    auto result = Q.solve(yvals);
    return result;
}

Eigen::VectorXd polyfit(std::vector<cv::Point> data, int degree) 
{       
    EASY_FUNCTION(profiler::colors::Red200); 

    // assert(xvals.size() == yvals.size());
    // assert(degree >= 1 && degree <= xvals.size() - 1);
    size_t N = data.size();

    Eigen::VectorXd xvals(N);
    Eigen::VectorXd yvals(N);
    
    for(size_t i=0; i<N; i++){
      // std::cout << i.x << "\n";
      xvals(i) = data[i].y;
      yvals(i) = data[i].x;
    }
    // std::cout << xvals << "\n";
    // std::cout << yvals << "\n";

    Eigen::MatrixXd A(N, degree + 1);
    A.col(0) = Eigen::VectorXd::Ones(N);

    for (Eigen::Index i = 0; i < degree; ++i)
        A.col(i + 1) = A.col(i).cwiseProduct(xvals);

    auto Q = A.householderQr();
    auto result = Q.solve(yvals);
    return result;
}

// Evaluate a polynomial.
double polyeval(Eigen::VectorXd coeffs, double x) {
  EASY_FUNCTION(profiler::colors::Red200); 

  double result = 0.0;
  for (int i = 0; i < coeffs.size(); i++) {
    // std::cout << coeffs[i] << " " << pow(x, i) << "\n";
    result += coeffs[i] * pow(x, i);
  }
  return result;
}
} // namespace fit_poly
