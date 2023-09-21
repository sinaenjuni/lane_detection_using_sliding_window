
# https://modoocode.com/332
# 정적 라이브러리 module을 만든다.
# add_library(module1 STATIC src/video.cpp src/sliding_window.cpp)
add_library(module1 STATIC 
                src/preprocessing.cc 
                src/sliding_window.cc 
                src/fit_poly.cc
                src/thread_pool.cc)

# 해당 라이브러리를 컴파일 할 옵션
# target_compile_options(module1 PRIVATE -Wall -Werror)

# 해당 라이브러리 컴파일 시 사용할 헤더파일 경로
target_include_directories(module1 PUBLIC includes)

find_package(OpenCV 4.4 REQUIRED PATHS ${PROJECT_SOURCE_DIR}/thirdparty/OpenCV/install/lib/cmake/opencv4)
if(NOT OpenCV_FOUND)
    # https://cmake.org/cmake/help/latest/command/message.html
    message(FATAL_ERROR "OpenCV > 4.4 not found.")
else()
    message(STATUS "Find OpenCV module")
    message(STATUS ${OpenCV_INCLUDE_DIRS})
    # target_link_libraries(module1 PUBLIC ${OpenCV_LIBS})
    # target_include_directories(module1 PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(module1 PUBLIC ${OpenCV_LIBS})
    target_include_directories(module1 PUBLIC ${OpenCV_INCLUDE_DIRS})
endif()

# add_executable(module1_test src/test.cpp)
# target_link_libraries(module1_test module1)


find_package (Eigen3 3.3 REQUIRED NO_MODULE)
if (Eigen3_FOUND)
    message(STATUS "Eigen3 Found! - ${Eigen3_DIR}")
    # set(EIGEN3_INCLUDE_DIRS "~/thirdparty/eigen3_3.1.0/install/include")
    set(EIGEN3_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/thirdparty/eigen3_3.1.0/install/release/include)
    
    # set(Eigen3_LIBS Eigen3::Eigen)
    message(STATUS "EIGEN3_INCLUDE_DIRS - ${EIGEN3_INCLUDE_DIRS}")
    message(STATUS "Eigen3_LIBS - ${Eigen3_LIBS}")

    # include_directories(${EIGEN3_INCLUDE_DIRS})
    # link_libraries(${Eigen3_LIBS})
    target_link_libraries(module1 PUBLIC Eigen3::Eigen)

endif(Eigen3_FOUND)

add_executable(fit_poly_test src/fit_poly_test.cc)
target_link_libraries(fit_poly_test module1)

find_package(easy_profiler REQUIRED PATHS ${PROJECT_SOURCE_DIR}/thirdparty/easy_profiler/install/release/lib/cmake/easy_profiler)
target_link_libraries(module1 PUBLIC easy_profiler)
