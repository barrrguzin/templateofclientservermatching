#include "user.hpp"

user::user(){};

user::user(size_t id, std::string name)
{
    userId = id;
    userName = name;
    balanceRUB = 0;
    balanceUSD = 0;
}

size_t& user::id()
{
    return userId;
}

std::string& user::name()
{
    return userName;
}

int64_t& user::usd()
{
    return balanceUSD;
}

int64_t& user::rub()
{
    return balanceRUB;
}