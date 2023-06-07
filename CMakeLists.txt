cmake_minimum_required(VERSION 3.5)

project(ZadanieSCR LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ZadanieSCR main.cpp
                          zadanie1.cpp
                          zadanie2.cpp
                          zadanie3.cpp)

install(TARGETS ZadanieSCR
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
