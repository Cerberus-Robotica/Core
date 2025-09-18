cmake_minimum_required(VERSION 3.20)

project(hades)

find_package(Protobuf REQUIRED)

set(lcm_DIR "${CMAKE_SOURCE_DIR}/../external/lcm/cmake")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
        src/team/geometry/Point.cpp
        src/team/geometry/Point.h
        src/team/geometry/LineSegment.cpp
        src/team/geometry/LineSegment.h
        src/team/geometry/Area.cpp
        src/team/geometry/Area.h
        src/team/geometry/AreaCircular.cpp
        src/team/geometry/AreaCircular.h
        src/team/geometry/AreaRectangular.cpp
        src/team/geometry/AreaRectangular.h
        src/team/geometry/Ball.cpp
        src/team/geometry/Ball.h
        src/team/geometry/Field.cpp
        src/team/geometry/Field.h
        src/team/geometry/Vector2d.cpp
        src/team/geometry/Vector2d.h
        src/team/geometry/VisibilityGraph.h
        src/team/geometry/VisibilityGraph.cpp
        src/team/geometry/Shadow.h
        src/team/geometry/Shadow.cpp
        src/team/geometry/CircularShadow.h
        src/team/geometry/CircularShadow.cpp
        src/team/geometry/geometry.h

        src/team/plays/PlayAttack.cpp
        src/team/plays/PlayAttack.h
        src/team/plays/PlayDefense.cpp
        src/team/plays/PlayDefense.h
        src/team/plays/PlayDebug.cpp
        src/team/plays/PlayDebug.h
        src/team/skills/skills.h
        src/team/tactics/tactics.h
        src/team/roles/roles.h
        src/team/plays/PlayHalt.cpp
        src/team/plays/PlayHalt.h
        src/team/plays/PlayOurKickOff.cpp
        src/team/plays/PlayOurKickOff.h
        src/team/plays/PlayTheirKickOff.cpp
        src/team/plays/PlayTheirKickOff.h
        src/team/plays/PlayBase.cpp
        src/team/plays/PlayBase.h


        src/team/roles/RoleBase.cpp
        src/team/roles/RoleBase.h
        src/team/roles/RoleHalted.cpp
        src/team/roles/RoleHalted.h
        src/team/roles/RoleSupport.cpp
        src/team/roles/RoleSupport.h
        src/team/roles/RoleGoalKeeper.cpp
        src/team/roles/RoleGoalKeeper.h
        src/team/roles/RoleDefender.cpp
        src/team/roles/RoleDefender.h
        src/team/roles/RoleKickOffGoalKeeper.cpp
        src/team/roles/RoleKickOffGoalKeeper.h
        src/team/roles/RoleKickOffKicker.cpp
        src/team/roles/RoleKickOffKicker.h
        src/team/roles/RoleKickOffSupport.cpp
        src/team/roles/RoleKickOffSupport.h
        src/team/roles/RoleStriker.cpp
        src/team/roles/RoleStriker.h
        src/team/roles/RoleSquaredTrajectory.h
        src/team/roles/RoleSquaredTrajectory.cpp
        src/team/roles/RoleCircularTrajectory.h
        src/team/roles/RoleCircularTrajectory.cpp
        src/team/roles/RoleMarker.h
        src/team/roles/RoleMarker.cpp
        src/team/roles/RoleRetaker.h
        src/team/roles/RoleRetaker.cpp


        src/team/tactics/TacticBase.h
        src/team/tactics/TacticBase.cpp
        src/team/tactics/TacticKeepXLine.h
        src/team/tactics/TacticKeepXLine.cpp
        src/team/tactics/TacticKeepLocation.h
        src/team/tactics/TacticKeepLocation.cpp
        src/team/tactics/TacticPositionAndKick.h
        src/team/tactics/TacticPositionAndKick.cpp
        src/team/tactics/TacticFollowTrajectory.h
        src/team/tactics/TacticFollowTrajectory.cpp
        src/team/tactics/TacticIntercept.h
        src/team/tactics/TacticIntercept.cpp
        src/team/tactics/TacticMark.h
        src/team/tactics/TacticMark.cpp
        src/team/tactics/tactics.h

        src/team/skills/SkillBase.h
        src/team/skills/SkillBase.cpp
        src/team/skills/SkillKick.h
        src/team/skills/SkillKick.cpp
        src/team/skills/SkillMoveTo.h
        src/team/skills/SkillMoveTo.cpp
        src/team/skills/SkillTurnTo.h
        src/team/skills/SkillTurnTo.cpp
        src/team/skills/SkillStop.h
        src/team/skills/SkillStop.cpp
        src/team/skills/SkillCushion.h
        src/team/skills/SkillCushion.cpp
        src/team/skills/skills.h

)

target_include_directories(${PROJECT_NAME}
                            PRIVATE 
                            ${LCM_INCLUDE_DIRS}
                            src/include
                            ${CMAKE_SOURCE_DIR}/../data_lcm/data)  

# Linkar bibliotecas
target_link_libraries(${PROJECT_NAME} lcm)
