cmake_minimum_required(VERSION 3.20)

project(BussinessHub)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow CONFIG REQUIRED)
find_package(CURL REQUIRED)

add_executable(BussinessHub
    backend/main.cpp
)

target_link_libraries(BussinessHub
    PRIVATE
    Crow::Crow
    CURL::libcurl
)