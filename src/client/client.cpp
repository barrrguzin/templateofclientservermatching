#include "client.hpp"

void Client::SendMessage(
        tcp::socket& aSocket,
        const std::string& aId,
        const std::string& aRequestType,
        const std::string& aMessage,
        bool isMessageObject)
{
    nlohmann::json req;
    req["UserId"] = aId;
    req["ReqType"] = aRequestType;
    if (isMessageObject)
        req["Message"] = nlohmann::json::parse(aMessage);
    else
        req["Message"] = aMessage;

    std::string request = req.dump();
    boost::asio::write(aSocket, boost::asio::buffer(request, request.size()));
}

// Возвращает строку с ответом сервера на последний запрос.
std::string Client::ReadMessage(tcp::socket& aSocket)
{
    boost::asio::streambuf b;
    boost::asio::read_until(aSocket, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}

std::string Client::SendOrder(tcp::socket& socket, const std::string& id, const std::string& orderType, const size_t& price, const size_t& amount)
{
    nlohmann::json message;
    message["OrderType"] = orderType;
    message["Price"] = price;
    message["Amount"] = amount;
    SendMessage(socket, id, Requests::Order, message.dump(), true);
    return ReadMessage(socket);
}

// "Создаём" пользователя, получаем его ID.
std::string Client::ProcessRegistration(tcp::socket& aSocket)
{
    std::string name;
    std::cout << "Hello! Enter your name: ";
    std::cin >> name;

    // Для регистрации Id не нужен, заполним его нулём
    SendMessage(aSocket, "0", Requests::Registration, name);
    return ReadMessage(aSocket);
}