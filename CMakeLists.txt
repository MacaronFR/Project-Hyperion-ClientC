cmake_minimum_required(VERSION 3.17)
project(clientc C)

set(CMAKE_C_STANDARD 99)

include_directories(/usr/include/libxml2)
include_directories(include)

add_executable(clientc src/main.c src/xml.c src/bdd.c)
target_link_libraries(clientc -lmariadb)
target_link_libraries(clientc -lxml2)