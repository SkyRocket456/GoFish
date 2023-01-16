cmake_minimum_required(VERSION 3.21)
project(GoFish)

set(CMAKE_CXX_STANDARD 14)

add_executable(GoFish go_fish.cpp card.cpp deck.cpp player.cpp RoboDealer.cpp)
