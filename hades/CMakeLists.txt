cmake_minimum_required(VERSION 3.20)

project(hades)

find_package(Protobuf REQUIRED)

set(lcm_DIR "/home/cerberus/.local/lib/python3.10/site-packages/lib/lcm/cmake")
#set(lcm_DIR "/usr/local/lib/lcm/cmake") #outra opção de diretorio

find_package(lcm REQUIRED)

# Adicionar executável
add_executable(${PROJECT_NAME} src/main.cpp
        src/team/Robot_controller.cpp
        src/team/Robot_controller.h
        src/team/leader.cpp
        src/team/leader.h
        src/c_trajectory/C_trajectory.cpp
        src/c_trajectory/C_trajectory.h
        src/c_trajectory/geometry/Vetop.cpp
        src/c_trajectory/geometry/Vetop.h
        src/c_trajectory/geometry/circle.cpp
        src/c_trajectory/geometry/circle.h
        src/c_trajectory/geometry/rectangle.cpp
        src/c_trajectory/geometry/rectangle.h
        src/team/robot.cpp
        src/team/robot.h
        src/include/handlers.cpp
        src/team/team_info.cpp
        src/team/team_info.h
        src/team/geometry/world.cpp
        src/team/geometry/world.h
        src/team/plays/play_attack.cpp
        src/team/plays/play_attack.h
        src/team/plays/play_goal_keeper.cpp
        src/team/plays/play_goal_keeper.h
        src/team/plays/play_debug.cpp
        src/team/plays/play_debug.h
)

target_include_directories(${PROJECT_NAME}
                            PRIVATE 
                            ${LCM_INCLUDE_DIRS}
                            src/include
                            ${CMAKE_SOURCE_DIR}/../data_lcm/data)  

# Linkar bibliotecas
target_link_libraries(${PROJECT_NAME} lcm)
