# CMake generated Testfile for 
# Source directory: C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/opencv/modules/ml
# Build directory: C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/opencv/modules/ml
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_ml "C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/bin/opencv_test_mld.exe" "--gtest_output=xml:opencv_test_ml.xml")
set_tests_properties(opencv_test_ml PROPERTIES  LABELS "Main;opencv_ml;Accuracy" WORKING_DIRECTORY "C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/test-reports/accuracy" _BACKTRACE_TRIPLES "C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/opencv/cmake/OpenCVUtils.cmake;1739;add_test;C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/opencv/cmake/OpenCVModule.cmake;1352;ocv_add_test_from_target;C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/opencv/cmake/OpenCVModule.cmake;1110;ocv_add_accuracy_tests;C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/opencv/modules/ml/CMakeLists.txt;2;ocv_define_module;C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/opencv/modules/ml/CMakeLists.txt;0;")
