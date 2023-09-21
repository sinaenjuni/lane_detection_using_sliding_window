#include "fit_poly.h"


// template <typename T>
// inline T horner_impl(T x, const T *coef, size_t count, T t) {
//     return count == 0 ? coef[count] + x * t
//                       : horner_impl(x, coef, count - 1, coef[count] + x * t);
// }
// /// Evaluate a polynomial using [Horner's method](https://en.wikipedia.org/wiki/Horner%27s_method).
// template <typename T, size_t N>
// inline T horner(T x, const T (&coef)[N]) {
//     return horner_impl(x, coef, N - 2, coef[N - 1]);
// }

// template <size_t N>
// void test(double (&coef)[N]){
//     std::cout << N << "\n";
//     for(auto i : coef)
//         std::cout << i << "\n";
// }

// int main() {

//     // std::cout << "test";

//     // Eigen 라이브러리의 버전 정보는 Macros.h 파일에 정의되어 있습니다.
//     // 이를 통해 버전 정보를 출력할 수 있습니다.
//     std::cout << "Eigen version: " << EIGEN_WORLD_VERSION << "."
//               << EIGEN_MAJOR_VERSION << "."
//               << EIGEN_MINOR_VERSION << std::endl;

//     double coef[] = {1, -0.5, 0.3, -0.2, 0.1};
//     auto eval_poly = [&](double x) { return horner(x, coef); };
//     // auto result = [캡처](매개 변수){함수 동작}(호출)

//     // test(coef);


//     const Eigen::Index N = 20;
//     Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(N, -1., +1.);
//     std::cout << "X: " << x << "\n";
//     Eigen::VectorXd y(N);
//     std::cout <<"Y: " << y << "\n";
//     std::transform(std::begin(x), std::end(x), std::begin(y), eval_poly);
//     std::cout << y << "\n";

//    // Generate the Vandermonde matrix
//     const Eigen::Index degree = 4;
//     Eigen::MatrixXd V(N, degree + 1);
//     V.col(0) = Eigen::VectorXd::Ones(N);
//     for (Eigen::Index i = 0; i < degree; ++i)
//         V.col(i + 1) = V.col(i).cwiseProduct(x);
//     std::cout << V << "\n";

//     // // Scale the problem
//     Eigen::VectorXd scale = V.colwise().norm().cwiseInverse();
//     V *= scale.asDiagonal();
//     std::cout.precision(10);
//     std::cout << scale.transpose() << std::endl;
    
//     // Solve the Vandermonde system using QR decomposition
//     Eigen::VectorXd solution = V.colPivHouseholderQr().solve(y);
//     solution.transpose() *= scale.asDiagonal();
//     std::cout << solution.transpose() << std::endl;

//     // Solve the Vandermonde system using singular value decomposition
//     Eigen::VectorXd solution2 = 
//         V.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(y);
//     solution2.transpose() *= scale.asDiagonal();
//     std::cout << solution2.transpose() << std::endl;

//     return 0;
// }



/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////   Test block

int main() 
{
    Eigen::VectorXd xvals(6);
    Eigen::VectorXd yvals(6);
    // x waypoint coordinates
    xvals << 9.261977, -2.06803, -19.6663, -36.868, -51.6263, -66.3482;
    // y waypoint coordinates
    yvals << 5.17, -2.25, -15.306, -29.46, -42.85, -57.6116;

    // TODO: use `polyfit` to fit a third order polynomial to the (x, y)
    // coordinates.
    auto coeff = fit_poly::polyfit(xvals, yvals, 2); // 2차식 (0:상수, 1:x, 2:x^2)
    std::cout << coeff <<"\n";
    std::cout <<"\n";


    // for (double x = 0; x <= 20; x += 1.0) {
    // // TODO: use `polyeval` to evaluate the x values.
    //     std::cout << polyeval(coeff,x) << std::endl; 
    // }
    for (auto x : xvals) {
    // TODO: use `polyeval` to evaluate the x values.
        std::cout << fit_poly::polyeval(coeff,x) << std::endl; 
    }
  
}