#include <iostream>
#include <server.hpp>
#include <boost/asio/io_service.hpp>

#include "json.hpp"
#include "core.hpp"

int main(int argc, char** argv)
{
    try
    {
        boost::asio::io_service io_service;
        static Core core;

        server s(io_service);

        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}