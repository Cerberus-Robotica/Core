cmake_minimum_required(VERSION 3.10)
project(lcm_cpp_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(${PROJECT_SOURCE_DIR}/../../../../../data_lcm/data)

find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LCM REQUIRED lcm)

add_executable(server main.cpp)

target_include_directories(server PRIVATE ${LCM_INCLUDE_DIRS})
target_link_libraries(server PRIVATE ${LCM_LIBRARIES} Threads::Threads)
