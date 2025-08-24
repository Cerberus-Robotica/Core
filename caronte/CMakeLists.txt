cmake_minimum_required(VERSION 3.20)

project(caronte)

find_package(Protobuf REQUIRED)

set(lcm_DIR "${CMAKE_SOURCE_DIR}/../external/lcm/cmake")
#set(lcm_DIR "/usr/local/lib/lcm/cmake") #outra opção de diretorio
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../cmake")

#find_package(lcm REQUIRED)

message(STATUS "Using Protobuf ${Protobuf_VERSION}")

set(PROTO_FILES
    ${CMAKE_SOURCE_DIR}/../proto/ssl_gc_referee_message.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_vision_wrapper.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_gc_geometry.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_vision_detection.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_vision_geometry.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_gc_common.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_gc_engine.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_gc_game_event.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_vision_wrapper_tracked.proto
    ${CMAKE_SOURCE_DIR}/../proto/ssl_vision_detection_tracked.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# Adicionar executável
add_executable(${PROJECT_NAME} src/main.cpp   
                        src/GC.cpp
                        src/vision.cpp
                        src/tartarus.cpp
                        src/socket_connect.cpp
                        src/autoref.cpp
                        ${PROTO_SRCS}
                        )

target_include_directories(${PROJECT_NAME} 
                            PRIVATE 
                            ${Protobuf_INCLUDE_DIRS} 
                            ${LCM_INCLUDE_DIRS} 
                            src/include 
                            ${CMAKE_SOURCE_DIR}/../data_lcm/data)  

# Linkar bibliotecas
target_link_libraries(${PROJECT_NAME} ${Protobuf_LIBRARIES} lcm)

target_compile_options(${PROJECT_NAME} PRIVATE -w)