cmake_minimum_required(VERSION 3.8)
project(MemeCrawler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_FILES main.cpp Requester.cpp Requester.h JSONFilter.cpp JSONFilter.h RepostFilter.cpp RepostFilter.h ImageHasher.cpp ImageHasher.h DatabaseHandler.cpp DatabaseHandler.h)
add_executable(MemeCrawler ${SOURCE_FILES})
target_link_libraries(MemeCrawler curl curlpp opencv_core opencv_imgproc opencv_highgui opencv_img_hash opencv_imgcodecs libmysqlcppconn-static.so dl)
LINK_DIRECTORIES(/usr/local/lib)
set( CMAKE_CXX_FLAGS " -pthread " )