cmake_minimum_required(VERSION 3.20)

project(hermes)

find_package(Protobuf REQUIRED)

#find_package(serial REQUIRED)

find_package(SDL2 REQUIRED)

set(lcm_DIR "${CMAKE_SOURCE_DIR}/../external/lcm/cmake")
#set(lcm_DIR "/usr/local/lib/lcm/cmake") #outra opção de diretorio
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../cmake")

#find_package(lcm REQUIRED)

message(STATUS "Using Protobuf ${Protobuf_VERSION}")

set(PROTO_FILES
    ${CMAKE_SOURCE_DIR}/../proto/grSim_Commands.proto
    ${CMAKE_SOURCE_DIR}/../proto/grSim_Packet.proto
    ${CMAKE_SOURCE_DIR}/../proto/grSim_Replacement.proto
    ${CMAKE_SOURCE_DIR}/../proto/grSim_Robotstatus.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# Adicionar executável
add_executable(${PROJECT_NAME}
                src/main.cpp
                ${PROTO_SRCS}
                src/include/socket_connect.cpp
                src/include/handler.cpp
                src/include/esp.cpp
                src/include/send_to_robots.cpp
                src/include/esp_connect.cpp
                src/include/manual_control.cpp
                src/include/stm_connect.cpp
                )

target_include_directories(${PROJECT_NAME}
                            PRIVATE 
                            ${Protobuf_INCLUDE_DIRS} 
                            ${SDL2_INCLUDE_DIRS}
                            ${LCM_INCLUDE_DIRS}
                            ${CMAKE_SOURCE_DIR}/../data_lcm/data
                            src/include                         
                            )  

# Linkar bibliotecas
target_link_libraries(${PROJECT_NAME} ${Protobuf_LIBRARIES} lcm ${SDL2_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE -w)
