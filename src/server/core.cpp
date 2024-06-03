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
        for (std::pair<int64_t, std::deque<order>> row : buyOrders)
        {
            auto q = row.second;
            for (order order : q)
                result.append("USD: " + std::to_string(order.amount_()) + "; Price RUB (one USD): " + std::to_string(order.price_()) + "\n");
        }
        result.append("---\nSell orders:\n");
        for (std::pair<int64_t, std::deque<order>> row : sellOrders)
        {
            auto q = row.second;
            for (order order : q)
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

void Core::ProcessSellOrder(order order_)
{
    if (ProcessSell(order_))
        return;

    const auto storedOrder = sellOrders.find(order_.price_());
    if (storedOrder == sellOrders.cend())
        sellOrders.emplace(order_.price_(), std::deque<order>({order_}));
    else
        storedOrder->second.push_back(order_);
}

// Обработка запроса на покупку USD
void Core::ProcessBuyOrder(order order_)
{
    if (ProcessBuy(order_))
        return;

    const auto storedOrder = buyOrders.find(order_.price_());
    if (storedOrder == buyOrders.cend())
        buyOrders.emplace(order_.price_(), std::deque<order>({order_}));
    else
        storedOrder->second.push_back(order_);
}

bool Core::ProcessSell(order order_)
{
    if (buyOrders.empty())
        return false;

    auto firstOrder = buyOrders.begin();
    order& matchedOrder = firstOrder->second.front();
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
                {
                    firstOrder->second.pop_front();
                    if (firstOrder->second.empty())
                        buyOrders.erase(firstOrder->first);
                }
                return true;
            }
            else
            {
                order_.amount_() - matchedOrder.amount_();
                sellerIt->second.rub() += matchedOrder.price_() * matchedOrder.amount_();
                sellerIt->second.usd() -= matchedOrder.amount_();
                buyerIt->second.rub() -= matchedOrder.price_() * matchedOrder.amount_();
                buyerIt->second.usd() += matchedOrder.amount_();
                firstOrder->second.pop_front();
                if (firstOrder->second.empty())
                    buyOrders.erase(firstOrder->first);
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

    auto firstOrder = sellOrders.begin();
    order& matchedOrder = firstOrder->second.front();
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
                {
                    firstOrder->second.pop_front();
                    if (firstOrder->second.empty())
                        sellOrders.erase(firstOrder->first);
                }
                return true;
            }
            else
            {
                order_.amount_() -= matchedOrder.amount_();
                sellerIt->second.rub() += matchedOrder.price_() * matchedOrder.amount_();
                sellerIt->second.usd() -= matchedOrder.amount_();
                buyerIt->second.rub() -= matchedOrder.price_() * matchedOrder.amount_();
                buyerIt->second.usd() += matchedOrder.amount_();
                firstOrder->second.pop_front();
                if (firstOrder->second.empty())
                    sellOrders.erase(firstOrder->first);
                return ProcessBuy(order_);
            }
        }
    }
    else
    {
        return false;
    }
}