cmake_minimum_required(VERSION 3.17)
project(Assignment2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Assignment2 anomaly_detection_util.h anomaly_detection_util.cpp AnomalyDetector.h SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h timeseries.cpp timeseries.h minCircle.cpp minCircle.h mainTest.cpp HybridAnomalyDetector.cpp HybridAnomalyDetector.h CLI.cpp CLI.h commands.h Server.cpp Server.h)