#ifndef ORDER_HPP
#define ORDER_HPP
#include <string>
#include <stdexcept>
#include "Common.hpp"

class order {
public:

    order(std::string owner, std::string type, int64_t price, int64_t amount);

    order(const order& source);

    order(const order&& source);

    order();

    order& operator=(order other);

    const size_t& owner_();

    std::string& type_();

    int64_t& price_();

    int64_t& amount_();

private:
    size_t owner;
    std::string type;
    int64_t price;
    int64_t amount;
};



#endif //ORDER_HPP
