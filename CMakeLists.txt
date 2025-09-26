cmake_minimum_required(VERSION 3.15)
project(21Game VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем папку include в пути поиска заголовков
include_directories(include)

# Явно перечисляем исходные файлы
set(SOURCES
    src/main.cpp
    src/Card.cpp
    src/Deck.cpp
    src/Hand.cpp
    src/Player.cpp
    src/Dealer.cpp
    src/Game.cpp
)

add_executable(21game ${SOURCES})