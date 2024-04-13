#include <string>
#include "ArgumentExtractor.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/StreamCopier.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"

struct ExtractParams {
    std::string keyValueDelimiter;
    std::string pairDelimiter;
    std::string quotingCharacter;
    std::string text;
};

std::string readRequestBody(Poco::Net::HTTPServerRequest &req) {
    std::istream& in = req.stream();
    std::string body;
    Poco::StreamCopier::copyToString(in, body);
    return body;
}

Poco::JSON::Object::Ptr parseJson(const std::string &str) {
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(str);
    return result.extract<Poco::JSON::Object::Ptr>();
}

ArgumentExtractor::ParsedArguments ArgumentExtractor::extract(const Poco::Net::HTTPServerRequest & req)
{
    std::string body = readRequestBody(const_cast<Poco::Net::HTTPServerRequest &>(req));
    Poco::JSON::Object::Ptr jsonObj = parseJson(body);
    ParsedArguments args;

    if (jsonObj->has("data"))
    {
        args.data = jsonObj->getValue<std::string>("data");
    }

    if (jsonObj->has("keyValueDelimiter"))
    {
        args.key_value_delimiter = jsonObj->getNullableValue<std::string>("keyValueDelimiter")[0];
    }

    if (jsonObj->has("pairDelimiters"))
    {
        auto pairDelimiters = jsonObj->getValue<std::string>("pairDelimiters");
        args.pair_delimiters = std::vector<char>(pairDelimiters.begin(), pairDelimiters.end());
    }

    if (jsonObj->has("quotingCharacter"))
        args.quoting_character = jsonObj->getNullableValue<std::string>("quotingCharacter");

    return args;
}
