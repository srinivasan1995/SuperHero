cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

add_definitions(-DMG_ENABLE_SSL)

project(sslclient)

find_package(OpenSSL REQUIRED)

set (OpenSSLDir "D:/tests/vcpkg/installed/x64-windows/include")

add_library(sslclient
include/main.hpp
include/mongoose.h
src/mongoose/
src/mongoose.c
)

target_include_directories(
 sslclient
 PUBLIC
   "include"
)

add_executable(final_out
 src/main.cpp
)

target_link_libraries(final_out sslclient OpenSSL::SSL OpenSSL::Crypto)