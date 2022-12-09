cmake_minimum_required(VERSION 3.0.0)
project(fanctrl_cpp VERSION 0.1.0)

include(CTest)
enable_testing()

set(SRC_LIST fanctrl.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
