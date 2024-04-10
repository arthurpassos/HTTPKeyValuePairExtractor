#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include <iostream>
#include <KeyValuePairExtractorBuilder.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Util/ServerApplication.h>

class MyRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest & req, Poco::Net::HTTPServerResponse & resp) override
    {
        resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        resp.setContentType("text/html");

        auto kvp_extractor = KeyValuePairExtractorBuilder().build();

        auto result = kvp_extractor->extract("name:arthur");

        std::ostream& out = resp.send();
        out << "<h1>Hello world!</h1>"
            << "<p>Count: "  << ++count         << "</p>"
            << "<p>Host: "   << req.getHost()   << "</p>"
            << "<p>Method: " << req.getMethod() << "</p>"
            << "<p>URI: "    << req.getURI()    << "</p>";

        for (const auto & [key, value] : result) {
            out << key << " -> " << value << std::endl;
        }

        out.flush();

        std::cout << std::endl
             << "Response sent for count=" << count
             << " and URI=" << req.getURI() << std::endl;
    }

private:
    static int count;
};

int MyRequestHandler::count = 0;

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    virtual Poco::Net::HTTPRequestHandler * createRequestHandler(const Poco::Net::HTTPServerRequest &)
    {
        return new MyRequestHandler;
    }
};

class SimpleServer : public Poco::Util::ServerApplication {
protected:
    int main(const std::vector<std::string> &) override
    {
        Poco::Net::HTTPServer s(new MyRequestHandlerFactory, Poco::Net::ServerSocket(9090), new Poco::Net::HTTPServerParams);

        s.start();
        std::cout << std::endl << "Server started" << std::endl;

        waitForTerminationRequest();  // wait for CTRL-C or kill

        std::cout << std::endl << "Shutting down..." << std::endl;
        s.stop();

        return Application::EXIT_OK;
    }
};

int main(int argc, char * argv[]) {

    SimpleServer app;
    return app.run(argc, argv);;
}
