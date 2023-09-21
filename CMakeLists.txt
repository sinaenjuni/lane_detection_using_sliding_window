cmake_minimum_required(VERSION 3.0.0)
project(Lane_detection VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()

add_subdirectory(src/module1)
# add_subdirectory(src/module2)


add_executable(main src/main.cpp)
# message(${CMAKE_SOURCE_DIR})
# target_link_libraries(main module1)
target_link_libraries(main module1)
# target_link_libraries(main fit_poly)
# target_include_directories(${PROJECT_NAME} src/module1/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
