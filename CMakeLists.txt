cmake_minimum_required(VERSION 3.10)
project(SensorFusion)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/include/eigen3)


add_executable(SensorFusion 
    src/main.cpp 
    src/SensorDataParser.cpp 
    src/IMUDataParser.cpp 
    src/KalmanFilter.cpp 
    src/Utils.cpp
    src/Cluster.cpp
)

target_include_directories(SensorFusion PRIVATE include)
