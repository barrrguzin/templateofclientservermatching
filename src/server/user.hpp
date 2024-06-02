#ifndef USER_HPP
#define USER_HPP
#include <string>


class user
{
public:

    user();

    user(size_t id, std::string name);

    size_t& id();

    std::string& name();

    int64_t& usd();

    int64_t& rub();

private:
    size_t userId;
    std::string userName;
    int64_t balanceUSD;
    int64_t balanceRUB;
};



#endif //USER_HPP
