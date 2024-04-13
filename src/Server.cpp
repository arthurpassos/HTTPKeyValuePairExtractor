#include "Server.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "KeyValuePairExtractorBuilder.h"
#include "util/ArgumentExtractor.h"

class ExtractKeyValuePairsHTTPHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest & req, Poco::Net::HTTPServerResponse &resp) override {
        if (req.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) {

            auto & istream = req.stream();

            resp.setContentType("application/json");
            std::ostream& out = resp.send();

            while (!istream.eof())
            {
                auto extract_line_from_stream = [&istream]() -> std::string {
                    std::string line;
                    std::getline(istream, line);
                    return line;
                };

                auto line = extract_line_from_stream();

                auto extractor = KeyValuePairExtractorBuilder().build();

                auto result = extractor->extract(line);

                for (const auto & pair : result)
                {
                    std::cout<< pair.first << " : " << pair.second << std::endl;
                    out << pair.first << " : " << pair.second << std::endl;
                }
            }

            out.flush();
        }

        resp.setStatus(Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
    }
};

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &request) override {
        if (request.getURI() == "/extract-kv")
        {
            return new ExtractKeyValuePairsHTTPHandler;
        }

        return nullptr;
    }
};

int Server::main(const std::vector<std::string> & args) {
    Poco::Net::HTTPServer s(new MyRequestHandlerFactory, Poco::Net::ServerSocket(9090), new Poco::Net::HTTPServerParams);

    s.start();
    std::cout << std::endl << "Server started" << std::endl;

    waitForTerminationRequest();  // wait for CTRL-C or kill

    std::cout << std::endl << "Shutting down..." << std::endl;
    s.stop();

    return Application::EXIT_OK;
}
