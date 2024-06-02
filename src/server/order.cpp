#include "order.hpp"

order::order(std::string owner, std::string type, int64_t price, int64_t amount)
{
    if (type != Order::Sell && type != Order::Buy)
        throw std::invalid_argument("Undifined order type");
    this->owner = std::stoi(owner);
    this->type = type;
    this->price = price;
    this->amount = amount;
}

order::order(const order& source)
: owner(source.owner), type(source.type), price(source.price), amount(source.amount)
{}

order::order(const order&& source)
: owner(std::move(source.owner)), type(std::move(source.type)),
price(std::move(source.price)), amount(std::move(source.amount))
{}

order::order() {};

order& order::operator=(order other)
{
    this->owner = other.owner;
    this->type = other.type;
    this->price = other.price;
    this->amount = other.amount;
    return *this;
}

const size_t& order::owner_()
{
    return owner;
}

std::string& order::type_()
{
    return type;
}

int64_t& order::price_()
{
    return price;
}

int64_t& order::amount_()
{
    return amount;
}