CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(ClienServerEcn)

FIND_PACKAGE(Boost 1.83 COMPONENTS system REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

FIND_PACKAGE(GTest REQUIRED)

SET(CMAKE_CXX_STANDARD 17)

include_directories(src src/common src/client src/server)

ADD_EXECUTABLE(Server
        src/server/main.cpp
        src/server/session.cpp
        src/server/session.hpp
        src/server/server.cpp
        src/server/server.hpp
        src/server/core.cpp
        src/server/core.hpp
        src/server/user.cpp
        src/server/user.hpp
        src/common/Common.hpp
        src/common/json.hpp
        src/server/order.cpp
        src/server/order.hpp
)
TARGET_LINK_LIBRARIES(Server PRIVATE Threads::Threads ${Boost_LIBRARIES})

ADD_EXECUTABLE(Client
        src/client/main.cpp
        src/client/client.cpp
        src/client/client.hpp
        src/common/Common.hpp
        src/common/json.hpp
)
TARGET_LINK_LIBRARIES(Client PRIVATE Threads::Threads ${Boost_LIBRARIES})

ADD_EXECUTABLE(Tests
        src/Tests.cpp
        src/server/session.cpp
        src/server/session.hpp
        src/server/server.cpp
        src/server/server.hpp
        src/server/core.cpp
        src/server/core.hpp
        src/server/user.cpp
        src/server/user.hpp
        src/client/client.cpp
        src/client/client.hpp
        src/server/order.cpp
        src/server/order.hpp
        src/common/Common.hpp
        src/common/json.hpp
)
target_link_libraries(Tests GTest::Main)