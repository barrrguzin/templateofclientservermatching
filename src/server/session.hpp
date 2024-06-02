#ifndef SESSION_HPP
#define SESSION_HPP
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include "Common.hpp"
#include "core.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;

class session
{
public:
    session(boost::asio::io_service& io_service);

    tcp::socket& socket();

    void start();

    // Обработка полученного сообщения.
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

    void handle_write(const boost::system::error_code& error);

private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};



#endif //SESSION_HPP
