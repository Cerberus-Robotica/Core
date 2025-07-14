cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(core)

# Evita recompilar LCM várias vezes
set(BUILD_SHARED_LIBS ON)

# Protobuf
find_package(Protobuf REQUIRED)

# LCM pelo código-fonte
add_subdirectory(external/lcm)

# Módulos
add_subdirectory(hermes)
add_subdirectory(caronte)
add_subdirectory(hades)
