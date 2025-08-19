//
// Created by caiu on 09/04/25.
//

#ifndef TEAM_H
#define TEAM_H
#include <map>
#include <memory>

#include "roles/roles.h"



class TeamInfo {
public:

    enum role {
        unknown = -1,
        goal_keeper,
        striker,
        mid_field,
        defender,
        halted,
        kickoff_kicker,
        kickoff_goal_keeper,
        kickoff_support,
        debug_circular_trajectory,
        debug_squared_trajectory
    };

    std::shared_ptr<roles::RoleHalted> role_halted = std::make_shared<roles::RoleHalted>();
    std::shared_ptr<roles::RoleDefender> role_defender = std::make_shared<roles::RoleDefender>();
    std::shared_ptr<roles::RoleStriker> role_striker = std::make_shared<roles::RoleStriker>();
    std::shared_ptr<roles::RoleGoalKeeper> role_goal_keeper = std::make_shared<roles::RoleGoalKeeper>();
    std::shared_ptr<roles::RoleMidField> role_mid_field = std::make_shared<roles::RoleMidField>();
    std::shared_ptr<roles::RoleKickOffKicker> role_kick_off_kicker = std::make_shared<roles::RoleKickOffKicker>();
    std::shared_ptr<roles::RoleKickOffSupport> role_kick_off_support = std::make_shared<roles::RoleKickOffSupport>();
    std::shared_ptr<roles::RoleKickOffGoalKeeper> role_kick_off_goal_keeper = std::make_shared<roles::RoleKickOffGoalKeeper>();

    std::map<role, std::shared_ptr<roles::RoleBase>> role_map = {
        {unknown, role_halted},
        {halted, role_halted},
        {defender, role_defender},
        {striker, role_striker},
        {goal_keeper, role_goal_keeper},
        {mid_field, role_mid_field},
        {kickoff_kicker, role_kick_off_kicker},
        {kickoff_support, role_kick_off_support},
        {kickoff_goal_keeper, role_kick_off_goal_keeper}
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
        normal_start,
        force_start
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
    enum Command current_command = FORCE_START;
    double central_line_x = 0;
    int play = -1;
    color color = yellow;
    int game_state = -1;
    std::array<role, 16> roles = {unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown};
    std::array<role, 16> enemy_roles = {unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown, unknown};
    //std::map<role, int> enemy_roles;

    double striker_max_dislocation = 1500;
    double mid_field_max_dislocation = 0;
    double defender_min_dislocation = 1500;

    //int roles[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::array<int, 16> active_robots = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int num_of_active_robots = 0;
    bool positioned[16] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
};



#endif //TEAM_H
