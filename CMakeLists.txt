cmake_minimum_required(VERSION 3.27)
project(HTTPKeyValuePairExtractor)

set(CMAKE_CXX_STANDARD 20)

add_executable(HTTPKeyValuePairExtractor main.cpp)

add_subdirectory(dependencies)

target_link_libraries(HTTPKeyValuePairExtractor KeyValuePairExtractorLib Poco::Net Poco::Foundation Poco::Util)
