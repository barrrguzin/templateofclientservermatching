#include "core.hpp"

Core& Core::GetCore()
{
    static Core core;
    return core;
}

// "Регистрирует" нового пользователя и возвращает его ID.
std::string Core::RegisterNewUser(const std::string& aUserName)
{
    size_t newUserId = mUsers.size();
    mUsers[newUserId] = std::move(user(newUserId, aUserName));
    return std::to_string(newUserId);
}

// Запрос имени клиента по ID
std::string Core::GetUserName(const std::string& aUserId)
{
    const auto userIt = mUsers.find(std::stoi(aUserId));
    if (userIt == mUsers.cend())
    {
        throw std::invalid_argument("Error! Unknown User");
    }
    else
    {
        return userIt->second.name();
    }
}

std::string Core::GetUserBalance(const std::string& aUserId)
{
    const auto userIt = mUsers.find(std::stoi(aUserId));
    if (userIt == mUsers.cend())
    {
        throw std::invalid_argument("Error! Unknown User");
    }
    else
    {
        std::string balance = "RUB: " + std::to_string(userIt->second.rub())
        + "; USD: " + std::to_string(userIt->second.usd());
        return balance;
    }
}

std::string Core::GetListOfOrders(const std::string& aUserId)
{
    const auto userIt = mUsers.find(std::stoi(aUserId));
    if (userIt == mUsers.cend())
    {
        throw std::invalid_argument("Error! Unknown User");
    }
    else
    {
        std::string result = "***\n";
        result.append("Buy orders:\n");
        for (order order : buyOrders)
        {
            result.append("USD: " + std::to_string(order.amount_()) + "; Price RUB (one USD): " + std::to_string(order.price_()) + "\n");
        }
        result.append("---\nSell orders:\n");
        for (order order : sellOrders)
        {
            result.append("USD: " + std::to_string(order.amount_()) + "; Price RUB (one USD): " + std::to_string(order.price_()) + "\n");
        }
        result.append("***\n");
        return result;
    }
}

void Core::ProcessOrder(order order)
{
    const auto userIt = mUsers.find(order.owner_());
    if (userIt == mUsers.cend())
    {
        throw std::invalid_argument("Error! Unknown User");
    }
    else
    {
        if (order.type_() == Order::Sell)
            ProcessSellOrder(std::move(order));
        else if (order.type_() == Order::Buy)
            ProcessBuyOrder(std::move(order));
        else
            throw std::invalid_argument("Unable to process order: Unknown order type");
    }
}



void Core::ProcessSellOrder(order order)
{
    if (ProcessSell(order))
        return;

    if (sellOrders.empty())
    {
        sellOrders.push_back(std::move(order));
    }
    else
    {
        size_t orderPlace = FindOrderPlace(order);
        if (orderPlace < sellOrders.size())
            sellOrders.insert(sellOrders.begin() + orderPlace, std::move(order));
        else
            sellOrders.push_back(std::move(order));
    }
}

// Обработка запроса на покупку USD
void Core::ProcessBuyOrder(order order)
{
    if (ProcessBuy(order))
        return;

    if (buyOrders.empty())
    {
        buyOrders.push_back(std::move(order));
    }
    else
    {
        size_t orderPlace = FindOrderPlace(order);
        if (orderPlace < buyOrders.size())
            buyOrders.insert(buyOrders.begin() + orderPlace, std::move(order));
        else
            buyOrders.push_back(std::move(order));
    }
}

size_t Core::FindOrderPlace(order& order)
{
    if (order.type_() == Order::Buy)
    {
        int left = 0;
        int right = buyOrders.size();
        int mid = 0;
        while (left <= right)
        {
            mid = (left + right)/2;
            if (order.price_() < buyOrders[mid].price_())
                right = mid - 1;
            else if (order.price_() > buyOrders[mid].price_())
                left = mid + 1;
            else
                return mid;
        }

        if (order.price_() < buyOrders[mid].price_())
            return mid;
        else
            return mid + 1;
    }
    if (order.type_() == Order::Sell)
    {
        int left = 0;
        int right = sellOrders.size();
        int mid = 0;
        while (left <= right)
        {
            mid = (left + right)/2;
            if (order.price_() < sellOrders[mid].price_())
                left = mid + 1;
            else if (order.price_() > sellOrders[mid].price_())
                right = mid - 1;
            else
                return mid;
        }

        if (order.price_() > sellOrders[mid].price_())
            return mid;
        else
            return mid + 1;
    }
    else
        throw std::invalid_argument("Undifined order type");
}

bool Core::ProcessSell(order order_)
{
    if (buyOrders.empty())
        return false;

    order& matchedOrder = buyOrders.back();
    if (matchedOrder.price_() >= order_.price_())
    {
        const auto buyerIt = mUsers.find(matchedOrder.owner_());
        const auto sellerIt = mUsers.find(order_.owner_());
        if (buyerIt == mUsers.cend() || sellerIt == mUsers.cend())
        {
            throw std::invalid_argument("Unable to find owner of processed order");
        }
        else
        {
            if (matchedOrder.amount_() >= order_.amount_())
            {
                matchedOrder.amount_() - order_.amount_();
                sellerIt->second.rub() += matchedOrder.price_() * order_.amount_();
                sellerIt->second.usd() -= order_.amount_();
                buyerIt->second.rub() -= matchedOrder.price_() * order_.amount_();
                buyerIt->second.usd() += order_.amount_();
                if (matchedOrder.amount_() == order_.amount_())
                    buyOrders.pop_back();

                return true;
            }
            else
            {
                order_.amount_() - matchedOrder.amount_();
                sellerIt->second.rub() += matchedOrder.price_() * matchedOrder.amount_();
                sellerIt->second.usd() -= matchedOrder.amount_();
                buyerIt->second.rub() -= matchedOrder.price_() * matchedOrder.amount_();
                buyerIt->second.usd() += matchedOrder.amount_();
                buyOrders.pop_back();
                return ProcessSell(order_);
            }
        }
    }
    else
    {
        return false;
    }
}

bool Core::ProcessBuy(order order_)
{
    if (sellOrders.empty())
        return false;

    order& matchedOrder = sellOrders.back();
    if (matchedOrder.price_() <= order_.price_())
    {
        const auto sellerIt = mUsers.find(matchedOrder.owner_());
        const auto buyerIt = mUsers.find(order_.owner_());
        if (buyerIt == mUsers.cend() || sellerIt == mUsers.cend())
        {
            throw std::runtime_error("Unable to find owner of processed order");
        }
        else
        {
            if (matchedOrder.amount_() >= order_.amount_())
            {
                matchedOrder.amount_() -= order_.amount_();
                sellerIt->second.rub() += matchedOrder.price_() * order_.amount_();
                sellerIt->second.usd() -= order_.amount_();
                buyerIt->second.rub() -= matchedOrder.price_() * order_.amount_();
                buyerIt->second.usd() += order_.amount_();
                if (matchedOrder.amount_() == order_.amount_())
                    sellOrders.pop_back();

                return true;
            }
            else
            {
                order_.amount_() -= matchedOrder.amount_();
                sellerIt->second.rub() += matchedOrder.price_() * matchedOrder.amount_();
                sellerIt->second.usd() -= matchedOrder.amount_();
                buyerIt->second.rub() -= matchedOrder.price_() * matchedOrder.amount_();
                buyerIt->second.usd() += matchedOrder.amount_();
                sellOrders.pop_back();
                return ProcessBuy(order_);
            }
        }
    }
    else
    {
        return false;
    }
}