#ifndef CORE_HPP
#define CORE_HPP
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include "Common.hpp"
#include "order.hpp"
#include "user.hpp"


class Core
{
public:
    static Core& GetCore();

    // "Регистрирует" нового пользователя и возвращает его ID.
    std::string RegisterNewUser(const std::string& aUserName);

    // Запрос имени клиента по ID
    std::string GetUserName(const std::string& aUserId);

    std::string GetUserBalance(const std::string& aUserId);

    std::string GetListOfOrders(const std::string& aUserId);

    void ProcessOrder(order order);

private:
    std::map<size_t, user> mUsers;
    //Отсортирован по убыванию
    std::vector<order> sellOrders;
    //Отсортирован по возрастанию
    std::vector<order> buyOrders;

    // Обработка запроса на продажу USD
    void ProcessSellOrder(order order);

    // Обработка запроса на покупку USD
    void ProcessBuyOrder(order order);

    size_t FindOrderPlace(order& order);

    bool ProcessSell(order order_);

    bool ProcessBuy(order order_);
};



#endif //CORE_HPP
