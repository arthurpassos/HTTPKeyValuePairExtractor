#pragma once

#include <Poco/Util/ServerApplication.h>

class Server : public Poco::Util::ServerApplication
{
public:

protected:
    int main(const std::vector<std::string> & args) override;
};
