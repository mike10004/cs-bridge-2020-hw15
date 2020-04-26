cmake_minimum_required(VERSION 3.7)
project(hw15)

add_subdirectory("${PROJECT_SOURCE_DIR}/payroll" "${PROJECT_SOURCE_DIR}/payroll/cmake-build")
add_subdirectory("${PROJECT_SOURCE_DIR}/payroll_tests" "${PROJECT_SOURCE_DIR}/payroll_tests/cmake-build")

add_subdirectory("${PROJECT_SOURCE_DIR}/q1" "${PROJECT_SOURCE_DIR}/q1/cmake-build")
