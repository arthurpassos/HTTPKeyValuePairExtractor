#pragma once

#include <string>
#include <optional>
#include <vector>
#include "Poco/Net/HTTPServerRequest.h"

class ArgumentExtractor
{
public:
    using CharArgument = std::optional<char>;
    using VectorArgument = std::vector<char>;

    struct ParsedArguments
    {
        std::string data;

        CharArgument key_value_delimiter = {};
        VectorArgument pair_delimiters = {};
        CharArgument quoting_character = {};
    };

    static ParsedArguments extract(const Poco::Net::HTTPServerRequest & req);
};
