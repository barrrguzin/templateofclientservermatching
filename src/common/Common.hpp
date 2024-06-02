#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>

static short port = 5555;

namespace Requests
{
    static std::string Registration = "Reg";
    static std::string Hello = "Hel";
    static std::string Balance = "Bal";
    static std::string Order = "Ord";
    static std::string List = "Lst";
}

namespace Order
{
    static std::string Sell = "sell";
    static std::string Buy = "buy";
}

#endif //CLIENSERVERECN_COMMON_HPP
