#include "session.hpp"

session::session(boost::asio::io_service& io_service)
        : socket_(io_service)
    {
    }

    tcp::socket& session::socket()
    {
        return socket_;
    }

    void session::start()
    {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&session::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    // Обработка полученного сообщения.
    void session::handle_read(const boost::system::error_code& error,
        size_t bytes_transferred)
    {
        if (!error)
        {
            data_[bytes_transferred] = '\0';

            // Парсим json, который пришёл нам в сообщении.
            auto j = nlohmann::json::parse(data_);
            auto reqType = j["ReqType"];

            std::string reply = "Error! Unknown request type";

            try
            {
                if (reqType == Requests::Registration)
                {
                    // Это реквест на регистрацию пользователя.
                    // Добавляем нового пользователя и возвращаем его ID.
                    reply = Core::GetCore().RegisterNewUser(j["Message"]);
                }
                else if (reqType == Requests::Hello)
                {
                    // Это реквест на приветствие.
                    // Находим имя пользователя по ID и приветствуем его по имени.
                    reply = "Hello, " + Core::GetCore().GetUserName(j["UserId"]) + "!\n";
                }
                else if (reqType == Requests::Balance)
                {
                    reply = "Balance: \n" + Core::GetCore().GetUserBalance(j["UserId"]) + "\n";
                }
                else if (reqType == Requests::Order)
                {
                    auto m = nlohmann::json::parse(j["Message"].dump());
                    order order(j["UserId"], m["OrderType"], m["Price"], m["Amount"]);
                    Core::GetCore().ProcessOrder(order);
                    reply = "Ok\n";
                }
                else if (reqType == Requests::List)
                {
                    reply = Core::GetCore().GetListOfOrders(j["UserId"]);
                }
            }
            catch (const std::exception& e)
            {
                reply = "Error: " + std::string(e.what()) + "\n";
            }

            boost::asio::async_write(socket_,
                boost::asio::buffer(reply, reply.size()),
                boost::bind(&session::handle_write, this,
                    boost::asio::placeholders::error));
        }
        else
        {
            delete this;
        }
    }

    void session::handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                boost::bind(&session::handle_read, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            delete this;
        }
    }