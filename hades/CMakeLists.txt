cmake_minimum_required(VERSION 3.20)

project(hades)

find_package(Protobuf REQUIRED)

set(lcm_DIR "${CMAKE_SOURCE_DIR}/../external/lcm/cmake")
#set(lcm_DIR "/usr/local/lib/lcm/cmake") #outra opção de diretorio
#list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/../cmake")

#find_package(lcm REQUIRED)

# Adicionar executável
add_executable(${PROJECT_NAME} src/main.cpp
        src/team/RobotController.cpp
        src/team/RobotController.h
        src/team/Leader.cpp
        src/team/Leader.h
        src/c_trajectory/C_trajectory.cpp
        src/c_trajectory/C_trajectory.h
        src/c_trajectory/geometry/Vetop.cpp
        src/c_trajectory/geometry/Vetop.h
        src/c_trajectory/geometry/Circle.cpp
        src/c_trajectory/geometry/Circle.h
        src/c_trajectory/geometry/Rectangle.cpp
        src/c_trajectory/geometry/Rectangle.h
        src/team/Robot.cpp
        src/team/Robot.h
        src/include/handlers.cpp
        src/team/TeamInfo.cpp
        src/team/TeamInfo.h
        src/team/geometry/WorldModel.cpp
        src/team/geometry/WorldModel.h
        src/team/plays/PlayAttack.cpp
        src/team/plays/PlayAttack.h
        src/team/plays/PlayDebug.cpp
        src/team/plays/PlayDebug.h
        src/team/RobotCalibration.cpp
        src/team/RobotCalibration.h
)

target_include_directories(${PROJECT_NAME}
                            PRIVATE 
                            ${LCM_INCLUDE_DIRS}
                            src/include
                            ${CMAKE_SOURCE_DIR}/../data_lcm/data)  

# Linkar bibliotecas
target_link_libraries(${PROJECT_NAME} lcm)
