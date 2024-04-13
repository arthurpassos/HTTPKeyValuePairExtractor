add_executable(HTTPKeyValuePairExtractor main.cpp Server.cpp
        util/ArgumentExtractor.cpp
)

target_link_libraries(HTTPKeyValuePairExtractor KeyValuePairExtractorLib Poco::Net Poco::Foundation Poco::Util)
