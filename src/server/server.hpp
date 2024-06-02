#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <session.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "Common.hpp"

using boost::asio::ip::tcp;

class server
{
public:
    server(boost::asio::io_service& io_service);

    void handle_accept(session* new_session, const boost::system::error_code& error);

private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};



#endif //SERVER_HPP
