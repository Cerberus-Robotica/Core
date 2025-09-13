//
// Created by caiu on 09/04/25.
//

#ifndef TEAM_H
#define TEAM_H
#include <map>
#include <array>
#include <memory>

#include "roles/roles.h"
#include "Robot.h"
#include "RobotController.h"

class TeamInfo {
public:

    std::shared_ptr<roles::RoleHalted> role_halted = std::make_shared<roles::RoleHalted>();
    std::shared_ptr<roles::RoleDefender> role_defender = std::make_shared<roles::RoleDefender>();
    std::shared_ptr<roles::RoleStriker> role_striker = std::make_shared<roles::RoleStriker>();
    std::shared_ptr<roles::RoleGoalKeeper> role_goal_keeper = std::make_shared<roles::RoleGoalKeeper>();
    std::shared_ptr<roles::RoleSupport> role_mid_field = std::make_shared<roles::RoleSupport>();
    std::shared_ptr<roles::RoleKickOffKicker> role_kick_off_kicker = std::make_shared<roles::RoleKickOffKicker>();
    std::shared_ptr<roles::RoleKickOffSupport> role_kick_off_support = std::make_shared<roles::RoleKickOffSupport>();
    std::shared_ptr<roles::RoleKickOffGoalKeeper> role_kick_off_goal_keeper = std::make_shared<roles::RoleKickOffGoalKeeper>();
    std::shared_ptr<roles::RoleCircularTrajectory> role_circular_trajectory = std::make_shared<roles::RoleCircularTrajectory>();
    std::shared_ptr<roles::RoleSquaredTrajectory> role_squared_trajectory = std::make_shared<roles::RoleSquaredTrajectory>();

    std::map<Robot::role, std::shared_ptr<roles::RoleBase>> role_map = {
        {Robot::unknown, role_halted},
        {Robot::halted, role_halted},
        {Robot::defender, role_defender},
        {Robot::striker, role_striker},
        {Robot::goal_keeper, role_goal_keeper},
        {Robot::support, role_mid_field},
        {Robot::kickoff_kicker, role_kick_off_kicker},
        {Robot::kickoff_support, role_kick_off_support},
        {Robot::kickoff_goal_keeper, role_kick_off_goal_keeper},
        {Robot::debug_circular_trajectory, role_circular_trajectory},
        {Robot::debug_squared_trajectory, role_squared_trajectory}
    };


    enum sides {
        left,
        right
    };

    enum color {
        blue,
        yellow
    };

    enum events {
        halt,
        timeout,
        stop,
        prepareOurKickOff,
        prepareTheirKickOff,
        ourballPlacement,
        theirballPlacement,
        prepareOurPenalty,
        prepareTheirPenalty,
        ourKickOff,
        theirKickOff,
        ourFreeKick,
        theirFreeKick,
        ourPenalty,
        theirPenalty,
        run
    };

    enum Command {
        // All robots should completely stop moving.
        HALT = 0,
        // Robots must keep 50 cm from the ball.
        STOP = 1,
        // A prepared kickoff or penalty may now be taken.
        NORMAL_START = 2,
        // The ball is dropped and free for either team.
        FORCE_START = 3,
        // The yellow team may move into kickoff position.
        PREPARE_KICKOFF_YELLOW = 4,
        // The blue team may move into kickoff position.
        PREPARE_KICKOFF_BLUE = 5,
        // The yellow team may move into penalty position.
        PREPARE_PENALTY_YELLOW = 6,
        // The blue team may move into penalty position.
        PREPARE_PENALTY_BLUE = 7,
        // The yellow team may take a direct free kick.
        DIRECT_FREE_YELLOW = 8,
        // The blue team may take a direct free kick.
        DIRECT_FREE_BLUE = 9,
        // The yellow team may take an indirect free kick.
        INDIRECT_FREE_YELLOW = 10,
        // The blue team may take an indirect free kick.
        INDIRECT_FREE_BLUE = 11,
        // The yellow team is currently in a timeout.
        TIMEOUT_YELLOW = 12,
        // The blue team is currently in a timeout.
        TIMEOUT_BLUE = 13,
        // The yellow team just scored a goal.
        // For information only.
        // Deprecated: Use the score field from the team infos instead. That way, you can also detect revoked goals.
        GOAL_YELLOW = 14,
        // The blue team just scored a goal. See also GOAL_YELLOW.
        GOAL_BLUE = 15,
        // Equivalent to STOP, but the yellow team must pick up the ball and
        // drop it in the Designated Position.
        BALL_PLACEMENT_YELLOW = 16,
        // Equivalent to STOP, but the blue team must pick up the ball and drop
        // it in the Designated Position.
        BALL_PLACEMENT_BLUE = 17
    };



    int goal_keeper_id = 0;
    sides our_side = right;
    int our_side_sign = +1;

    bool debug = false;
    enum Command current_command = HALT;
    enum events event = halt;
    double central_line_x = 0;
    color color = yellow;
    std::array<Robot::role, 16> roles = {Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown};
    std::array<Robot::role, 16> enemy_roles = {Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown, Robot::unknown};
    //std::map<role, int> enemy_roles;

    double striker_max_dislocation = 1500;
    double mid_field_max_dislocation = 0;
    double defender_min_dislocation = 1500;

    RobotController robots[16] = {
        RobotController(0), RobotController(1), RobotController(2), RobotController(3),
        RobotController(4), RobotController(5), RobotController(6), RobotController(7),
        RobotController(8), RobotController(9), RobotController(10), RobotController(11),
        RobotController(12), RobotController(13), RobotController(14), RobotController(15)
    };

    std::array<int, 16> active_robots = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int num_of_active_robots = 0;
    bool positioned[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

    Robot getRobotofRole(enum Robot::role role);
    Robot getEnemyofRole(enum Robot::role role, std::array<Robot, 16> enemies);
};



#endif //TEAM_H
