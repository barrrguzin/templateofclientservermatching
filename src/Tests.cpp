#include <gtest/gtest.h>
#include "core.hpp"
#include "order.hpp"

TEST(Core, RegisterUser)
{
    std::string id = Core::GetCore().RegisterNewUser("test1");
    ASSERT_NE(id, "");
}

TEST(Core, GetUsername)
{
    std::string name = "test";
    std::string id = Core::GetCore().RegisterNewUser(name);
    ASSERT_EQ(Core::GetCore().GetUserName(id), name);
}

TEST(Core, SellBuyOrders)
{
    std::string name = "test";
    std::string id1 = Core::GetCore().RegisterNewUser(name);
    order order1(id1, Order::Sell, 75, 100);
    std::string id2 = Core::GetCore().RegisterNewUser(name);
    order order2(id2, Order::Buy, 75, 100);

    Core::GetCore().ProcessOrder(order1);
    Core::GetCore().ProcessOrder(order2);

    ASSERT_EQ(Core::GetCore().GetUserBalance(id1), "RUB: 7500; USD: -100");
    ASSERT_EQ(Core::GetCore().GetUserBalance(id2), "RUB: -7500; USD: 100");
}

TEST(Core, ProcessMultiplePartialOrders)
{
    std::string name = "test";
    std::string id1 = Core::GetCore().RegisterNewUser(name);
    order order1(id1, Order::Sell, 75, 100);
    std::string id2 = Core::GetCore().RegisterNewUser(name);
    order order2(id2, Order::Buy, 100, 75);
    std::string id3 = Core::GetCore().RegisterNewUser(name);
    order order3(id3, Order::Buy, 75, 50);
    std::string id4 = Core::GetCore().RegisterNewUser(name);
    order order4(id4, Order::Sell, 100, 50);

    Core::GetCore().ProcessOrder(order1);
    EXPECT_EQ(Core::GetCore().GetUserBalance(id1), "RUB: 0; USD: 0");
    Core::GetCore().ProcessOrder(order2);
    EXPECT_EQ(Core::GetCore().GetUserBalance(id1), "RUB: 5625; USD: -75");
    EXPECT_EQ(Core::GetCore().GetUserBalance(id2), "RUB: -5625; USD: 75");
    Core::GetCore().ProcessOrder(order3);
    EXPECT_EQ(Core::GetCore().GetUserBalance(id1), "RUB: 7500; USD: -100");
    EXPECT_EQ(Core::GetCore().GetUserBalance(id2), "RUB: -5625; USD: 75");
    EXPECT_EQ(Core::GetCore().GetUserBalance(id3), "RUB: -1875; USD: 25");
    Core::GetCore().ProcessOrder(order4);
    EXPECT_EQ(Core::GetCore().GetUserBalance(id1), "RUB: 7500; USD: -100");
    EXPECT_EQ(Core::GetCore().GetUserBalance(id2), "RUB: -5625; USD: 75");
    EXPECT_EQ(Core::GetCore().GetUserBalance(id3), "RUB: -1875; USD: 25");
    EXPECT_EQ(Core::GetCore().GetUserBalance(id4), "RUB: 0; USD: 0");
}

TEST(Core, ListOrders)
{
    std::string name = "test";
    std::string target = "***\nBuy orders:\nUSD: 20; Price RUB (one USD): 5\nUSD: 50; Price RUB (one USD): 75\n---\n"
                         "Sell orders:\nUSD: 50; Price RUB (one USD): 100\n***\n";
    std::string id = Core::GetCore().RegisterNewUser(name);
    order order1(id, Order::Sell, 10, 10);
    order order2(id, Order::Buy, 5, 20);
    Core::GetCore().ProcessOrder(order1);
    Core::GetCore().ProcessOrder(order2);
    ASSERT_EQ(Core::GetCore().GetListOfOrders(id), target);
}

TEST(Core, GetUserBalance)
{
    std::string name = "test";
    std::string id = Core::GetCore().RegisterNewUser(name);
    ASSERT_EQ(Core::GetCore().GetUserBalance(id), "RUB: 0; USD: 0");
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
