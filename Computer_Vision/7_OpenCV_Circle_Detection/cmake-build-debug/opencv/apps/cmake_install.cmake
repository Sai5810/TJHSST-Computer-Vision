# Install script for directory: C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/opencv/apps

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files (x86)/mingw-w64/i686-8.1.0-posix-dwarf-rt_v6-rev0/mingw32/bin/objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/opencv/apps/annotation/cmake_install.cmake")
  include("C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/opencv/apps/visualisation/cmake_install.cmake")
  include("C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/opencv/apps/interactive-calibration/cmake_install.cmake")
  include("C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/opencv/apps/version/cmake_install.cmake")
  include("C:/Users/saiva/CLionProjects/Computer_Vision/7_OpenCV_Circle_Detection/cmake-build-debug/opencv/apps/model-diagnostics/cmake_install.cmake")

endif()

