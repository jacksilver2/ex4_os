cmake_minimum_required(VERSION 3.10)
project(os_ex4)

set(CMAKE_CXX_STANDARD 11)

add_executable(os_ex4 main.cpp server.cpp server.h client.cpp client.h whatsappUtils.cpp whatsappUtils.h test.cpp
        whatsappio.cpp whatsappio.h ser.cpp cl.cpp)