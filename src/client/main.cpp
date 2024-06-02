
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "client.hpp"
#include "Common.hpp"

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_service io_service;

        boost::asio::ip::tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        s.connect(*iterator);

        // Мы предполагаем, что для идентификации пользователя будет использоваться ID.
        // Тут мы "регистрируем" пользователя - отправляем на сервер имя, а сервер возвращает нам ID.
        // Этот ID далее используется при отправке запросов.
        std::string my_id = Client::ProcessRegistration(s);

        while (true)
        {
            // Тут реализовано "бесконечное" меню.
            std::cout << "Menu:\n"
                         "1) Hello Request\n"
                         "2) Balance\n"
                         "3) List of orders\n"
                         "4) Place sell order\n"
                         "5) Place buy order\n"
                         "6) Exit\n"
                         << std::endl;

            short menu_option_num;
            std::cin >> menu_option_num;
            switch (menu_option_num)
            {
                case 1:
                {
                    // Для примера того, как может выглядить взаимодействие с сервером
                    // реализован один единственный метод - Hello.
                    // Этот метод получает от сервера приветствие с именем клиента,
                    // отправляя серверу id, полученный при регистрации.
                    Client::SendMessage(s, my_id, Requests::Hello, "");
                    std::cout << Client::ReadMessage(s);
                    break;
                }
                case 2:
                {
                    Client::SendMessage(s, my_id, Requests::Balance, "");
                    std::cout << Client::ReadMessage(s);
                    break;
                }
                case 3:
                {
                    Client::SendMessage(s, my_id, Requests::List, "");
                    std::cout << Client::ReadMessage(s);
                    break;
                }
                case 4:
                {
                    std::string price;
                    std::string amount;
                    std::cout << "Enter the price per one USD in RUB: " << std::endl;
                    std::cin >> price;
                    std::cout << "Enter the amount of USD you want to sell: " << std::endl;
                    std::cin >> amount;
                    std::cout << Client::SendOrder(s, my_id, Order::Sell, std::stol(price), std::stol(amount));
                    break;
                }
                case 5:
                {
                    std::string price;
                    std::string amount;
                    std::cout << "Enter the price per one USD in RUB: " << std::endl;
                    std::cin >> price;
                    std::cout << "Enter the amount of USD you want to buy: " << std::endl;
                    std::cin >> amount;
                    std::cout << Client::SendOrder(s, my_id, Order::Buy, std::stol(price), std::stol(amount));
                    break;
                }
                case 6:
                {
                    exit(0);
                }
                default:
                {
                    std::cout << "Unknown menu option\n" << std::endl;
                }
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
