#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <boost/asio.hpp>

#include "Common.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;

class Client {
public:
    // Отправка сообщения на сервер по шаблону.
    static void SendMessage(tcp::socket& aSocket, const std::string& aId, const std::string& aRequestType,
        const std::string& aMessage, bool isMessageObject = false);

    // Возвращает строку с ответом сервера на последний запрос.
    static std::string ReadMessage(tcp::socket& aSocket);

    static std::string SendOrder(tcp::socket& socket, const std::string& id, const std::string& orderType,
        const size_t& price, const size_t& amount);

    // "Создаём" пользователя, получаем его ID.
    static std::string ProcessRegistration(tcp::socket& aSocket);
};



#endif //CLIENT_HPP
