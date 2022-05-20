cmake_minimum_required(VERSION 3.20)
project(Competition)

set(CMAKE_CXX_STANDARD 17)

add_executable(Competition main.cpp reader.h radix_sort.h Timer.h merge_sort.h trie.h)
